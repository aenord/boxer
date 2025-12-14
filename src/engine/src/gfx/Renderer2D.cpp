#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/Shader.h"
#include "engine/gfx/VertexArray.h"
#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/IndexBuffer.h"
#include "engine/gfx/Texture2D.h"
#include "engine/gfx/GLFunctions.h"
#include "engine/gfx/GLUtils.h"
#include "engine/gfx/Camera2D.h"
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_log.h>

namespace engine {

// Embedded shader sources for batched rendering
static const char* s_vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_texIndex;

uniform mat4 u_viewproj;

out vec2 v_uv;
out vec4 v_color;
flat out float v_texIndex;

void main() {
    v_uv = a_uv;
    v_color = a_color;
    v_texIndex = a_texIndex;
    gl_Position = u_viewproj * vec4(a_pos, 0.0, 1.0);
}
)";

static const char* s_fragmentShaderSource = R"(
#version 330 core
in vec2 v_uv;
in vec4 v_color;
flat in float v_texIndex;

uniform sampler2D u_textures[16];

out vec4 FragColor;

void main() {
    int index = int(v_texIndex);
    vec4 texColor = texture(u_textures[index], v_uv);
    FragColor = texColor * v_color;
}
)";

Renderer2D::Renderer2D() = default;
Renderer2D::~Renderer2D() {
    Shutdown();
}

bool Renderer2D::Init() {
    if (m_initialized) return true;
    
    // Verify OpenGL context exists before proceeding
    if (!IsGLContextValid()) {
        SDL_Log("Renderer2D::Init() called before OpenGL context was created!");
        return false;
    }
    
    // Load OpenGL functions
    if (!LoadGLFunctions()) {
        SDL_Log("Renderer2D: Failed to load OpenGL functions");
        return false;
    }
    
    // Enable alpha blending for transparent sprites
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERROR();
    
    CreateShader();
    if (!m_shader || !m_shader->IsValid()) {
        SDL_Log("Renderer2D: Failed to create shader");
        return false;
    }
    
    CreateQuadMesh();
    CreateDefaultTexture();
    GL_CHECK_ERROR();
    
    // Initialize texture slots (slot 0 = default white texture)
    m_textureSlots.fill(nullptr);
    m_textureSlots[0] = m_defaultTexture.get();
    
    m_initialized = true;
    SDL_Log("Renderer2D initialized (batched, max %u quads)", MAX_QUADS);
    return true;
}

void Renderer2D::Shutdown() {
    m_defaultTexture.reset();
    m_quadIBO.reset();
    m_quadVBO.reset();
    m_quadVAO.reset();
    m_shader.reset();
    m_initialized = false;
}

void Renderer2D::CreateShader() {
    m_shader = std::make_unique<Shader>(s_vertexShaderSource, s_fragmentShaderSource);
    
    // Set sampler uniforms once (texture unit indices never change)
    if (m_shader && m_shader->IsValid()) {
        m_shader->Bind();
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; ++i) {
            char name[20];
            snprintf(name, sizeof(name), "u_textures[%u]", i);
            m_shader->SetInt(name, static_cast<int>(i));
        }
        m_shader->Unbind();
    }
}

void Renderer2D::CreateQuadMesh() {
    // Reserve space for vertices
    m_vertices.reserve(MAX_VERTICES);
    
    // Create dynamic VBO (empty, will be filled each frame)
    m_quadVAO = std::make_unique<VertexArray>();
    m_quadVBO = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(QuadVertex), true);
    
    // Pre-generate all indices (pattern: 0,1,2, 2,3,0, 4,5,6, 6,7,4, ...)
    std::vector<uint32_t> indices(MAX_INDICES);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDICES; i += INDICES_PER_QUAD) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += VERTICES_PER_QUAD;
    }
    m_quadIBO = std::make_unique<IndexBuffer>(indices.data(), MAX_INDICES);
    
    // Configure VAO with batched vertex layout
    m_quadVAO->Bind();
    m_quadVBO->Bind();
    
    // Position (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, position));
    glEnableVertexAttribArray(0);
    // TexCoord (vec2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texCoord));
    glEnableVertexAttribArray(1);
    // Color (vec4)
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, color));
    glEnableVertexAttribArray(2);
    // TexIndex (float)
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, texIndex));
    glEnableVertexAttribArray(3);
    
    m_quadIBO->Bind();
    m_quadVAO->Unbind();
}

void Renderer2D::BeginFrame(const Camera2D& camera) {
    m_viewProjection = camera.GetViewProjectionMatrix();
    
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    
    StartBatch();
}

void Renderer2D::EndFrame() {
    Flush();
}

void Renderer2D::StartBatch() {
    m_vertices.clear();
    m_indexCount = 0;
    m_textureSlotIndex = 1;  // Reset (0 is default texture)
}

void Renderer2D::Flush() {
    if (m_vertices.empty()) return;
    
    // Upload vertex data
    m_quadVBO->SetData(m_vertices.data(), m_vertices.size() * sizeof(QuadVertex));
    
    // Bind shader and set uniforms
    m_shader->Bind();
    m_shader->SetMat4("u_viewproj", m_viewProjection);
    
    // Bind textures to all slots (unused slots get default texture)
    for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; ++i) {
        if (i < m_textureSlotIndex && m_textureSlots[i]) {
            m_textureSlots[i]->Bind(i);
        } else {
            m_defaultTexture->Bind(i);
        }
    }
    
    // Draw all quads in one call
    m_quadVAO->Bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    GL_CHECK_ERROR();
    
    m_shader->Unbind();
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color) {
    AddQuadToBatch(position, size, 0.0f, color, nullptr, Vec4(0.0f, 0.0f, 1.0f, 1.0f), Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, float rotation, const Vec4& color) {
    AddQuadToBatch(position, size, rotation, color, nullptr, Vec4(0.0f, 0.0f, 1.0f, 1.0f), Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture, 
                          const Vec4& tint) {
    AddQuadToBatch(position, size, 0.0f, tint, &texture, Vec4(0.0f, 0.0f, 1.0f, 1.0f), Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                          const Texture2D& texture, const Vec4& tint) {
    AddQuadToBatch(position, size, rotation, tint, &texture, Vec4(0.0f, 0.0f, 1.0f, 1.0f), Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture,
                          const Vec4& uvRect, const Vec4& tint) {
    AddQuadToBatch(position, size, 0.0f, tint, &texture, uvRect, Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                          const Texture2D& texture, const Vec4& uvRect, const Vec4& tint) {
    AddQuadToBatch(position, size, rotation, tint, &texture, uvRect, Flip::None);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture,
                          Flip flip, const Vec4& tint) {
    AddQuadToBatch(position, size, 0.0f, tint, &texture, Vec4(0.0f, 0.0f, 1.0f, 1.0f), flip);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                          const Texture2D& texture, Flip flip, const Vec4& tint) {
    AddQuadToBatch(position, size, rotation, tint, &texture, Vec4(0.0f, 0.0f, 1.0f, 1.0f), flip);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                          const Texture2D& texture, const Vec4& uvRect,
                          Flip flip, const Vec4& tint) {
    AddQuadToBatch(position, size, rotation, tint, &texture, uvRect, flip);
}

void Renderer2D::AddQuadToBatch(const Vec2& position, const Vec2& size, float rotation,
                                 const Vec4& color, const Texture2D* texture, const Vec4& uvRect,
                                 Flip flip) {
    if (!m_initialized) return;
    
    // Check if batch is full
    if (m_indexCount >= MAX_INDICES) {
        Flush();
        StartBatch();
    }
    
    // Find or assign texture slot
    float texIndex = 0.0f;  // Default texture
    if (texture && texture->IsValid()) {
        // Check if texture is already in a slot
        bool found = false;
        for (uint32_t i = 1; i < m_textureSlotIndex; ++i) {
            if (m_textureSlots[i] == texture) {
                texIndex = static_cast<float>(i);
                found = true;
                break;
            }
        }
        
        // Assign new slot if not found
        if (!found) {
            if (m_textureSlotIndex >= MAX_TEXTURE_SLOTS) {
                // Out of texture slots, flush and start new batch
                Flush();
                StartBatch();
            }
            texIndex = static_cast<float>(m_textureSlotIndex);
            m_textureSlots[m_textureSlotIndex] = texture;
            m_textureSlotIndex++;
        }
    }
    
    // Compute world-space vertex positions
    // Quad is centered at position, with given size
    float halfW = size.x * 0.5f;
    float halfH = size.y * 0.5f;
    
    // Local-space corner offsets (relative to center)
    Vec2 corners[4] = {
        Vec2(-halfW, -halfH),  // Bottom-left
        Vec2( halfW, -halfH),  // Bottom-right
        Vec2( halfW,  halfH),  // Top-right
        Vec2(-halfW,  halfH)   // Top-left
    };
    
    // Apply rotation if needed
    if (rotation != 0.0f) {
        for (int i = 0; i < 4; ++i) {
            corners[i] = corners[i].Rotated(rotation);
        }
    }
    
    // Translate to world position
    for (int i = 0; i < 4; ++i) {
        corners[i] = corners[i] + position;
    }
    
    // Extract UV coordinates from rect: (minU, minV, maxU, maxV)
    float minU = uvRect.x;
    float minV = uvRect.y;
    float maxU = uvRect.z;
    float maxV = uvRect.w;
    
    // Apply horizontal flip (swap left and right U coordinates)
    if (flip & Flip::Horizontal) {
        float temp = minU;
        minU = maxU;
        maxU = temp;
    }
    
    // Apply vertical flip (swap top and bottom V coordinates)
    if (flip & Flip::Vertical) {
        float temp = minV;
        minV = maxV;
        maxV = temp;
    }
    
    // Bottom-left
    m_vertices.push_back({
        corners[0],
        Vec2(minU, minV),
        color,
        texIndex
    });
    // Bottom-right
    m_vertices.push_back({
        corners[1],
        Vec2(maxU, minV),
        color,
        texIndex
    });
    // Top-right
    m_vertices.push_back({
        corners[2],
        Vec2(maxU, maxV),
        color,
        texIndex
    });
    // Top-left
    m_vertices.push_back({
        corners[3],
        Vec2(minU, maxV),
        color,
        texIndex
    });
    
    m_indexCount += INDICES_PER_QUAD;
}

void Renderer2D::CreateDefaultTexture() {
    // 1x1 white pixel (RGBA)
    // Bound when no texture is provided to satisfy OpenGL's requirement
    // that all sampler uniforms have valid textures
    uint8_t whitePixel[] = { 255, 255, 255, 255 };
    m_defaultTexture = std::make_unique<Texture2D>(whitePixel, 1, 1);
}

} // namespace engine

