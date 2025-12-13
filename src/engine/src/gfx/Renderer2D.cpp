#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/Shader.h"
#include "engine/gfx/VertexArray.h"
#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/IndexBuffer.h"
#include "engine/gfx/Texture2D.h"
#include "engine/gfx/GLFunctions.h"
#include "engine/gfx/Camera2D.h"
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_log.h>

namespace engine {

// Embedded shader sources
static const char* s_vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;

uniform mat4 u_viewproj;
uniform mat4 u_transform;

out vec2 v_uv;
out vec4 v_color;

void main() {
    v_uv = a_uv;
    v_color = a_color;
    gl_Position = u_viewproj * u_transform * vec4(a_pos, 0.0, 1.0);
}
)";

static const char* s_fragmentShaderSource = R"(
#version 330 core
in vec2 v_uv;
in vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform int u_useTexture;

out vec4 FragColor;

void main() {
    vec4 finalColor = v_color;
    if (u_useTexture == 1) {
        finalColor = texture(u_texture, v_uv) * v_color;
    }
    FragColor = finalColor * u_color;
}
)";

Renderer2D::Renderer2D() = default;
Renderer2D::~Renderer2D() {
    Shutdown();
}

bool Renderer2D::Init() {
    if (m_initialized) return true;
    
    // Load OpenGL functions
    if (!LoadGLFunctions()) {
        SDL_Log("Renderer2D: Failed to load OpenGL functions");
        return false;
    }
    
    CreateShader();
    if (!m_shader || !m_shader->IsValid()) {
        SDL_Log("Renderer2D: Failed to create shader");
        return false;
    }
    
    CreateQuadMesh();
    CreateDefaultTexture();
    
    m_initialized = true;
    SDL_Log("Renderer2D initialized");
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
}

void Renderer2D::CreateQuadMesh() {
    // Quad vertices: position (2), texcoord (2), color (4) = 8 floats per vertex
    // Unit quad centered at origin: -0.5 to 0.5
    float vertices[] = {
        // pos          uv        color (white)
        -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // bottom-left
         0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // bottom-right
         0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  // top-right
        -0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f   // top-left
    };
    
    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    
    m_quadVAO = std::make_unique<VertexArray>();
    m_quadVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_quadIBO = std::make_unique<IndexBuffer>(indices, 6);
    
    m_quadVAO->Bind();
    m_quadVBO->Bind();
    m_quadVAO->SetQuadLayout();
    m_quadIBO->Bind();
    m_quadVAO->Unbind();
}

void Renderer2D::BeginFrame(const Camera2D& camera) {
    m_viewProjection = camera.GetViewProjectionMatrix();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_shader->Bind();
    m_shader->SetMat4("u_viewproj", m_viewProjection);
    m_shader->SetInt("u_useTexture", 0);  // Default to no texture (will be set per-draw)
}

void Renderer2D::EndFrame() {
    m_shader->Unbind();
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color) {
    DrawQuadInternal(position, size, color, nullptr);
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture, 
                          const Vec4& tint) {
    DrawQuadInternal(position, size, tint, &texture);
}

void Renderer2D::DrawQuadInternal(const Vec2& position, const Vec2& size, const Vec4& color,
                                   const Texture2D* texture) {
    if (!m_initialized) return;
    
    // Build transform matrix: scale then translate (column-major order)
    // Matrix layout: [scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, 1, 0, posX, posY, 0, 1]
    Mat4 transform;
    transform.m[0] = size.x;   // scale X
    transform.m[1] = 0.0f;
    transform.m[2] = 0.0f;
    transform.m[3] = 0.0f;
    transform.m[4] = 0.0f;
    transform.m[5] = size.y;   // scale Y
    transform.m[6] = 0.0f;
    transform.m[7] = 0.0f;
    transform.m[8] = 0.0f;
    transform.m[9] = 0.0f;
    transform.m[10] = 1.0f;   // scale Z
    transform.m[11] = 0.0f;
    transform.m[12] = position.x;  // translation X
    transform.m[13] = position.y;  // translation Y
    transform.m[14] = 0.0f;
    transform.m[15] = 1.0f;
    
    // Set uniforms
    m_shader->SetMat4("u_transform", transform);
    m_shader->SetVec4("u_color", color);
    
    // Handle texture - always bind something to satisfy macOS OpenGL driver
    if (texture && texture->IsValid()) {
        texture->Bind(0);
        m_shader->SetInt("u_useTexture", 1);
    } else {
        m_defaultTexture->Bind(0);
        m_shader->SetInt("u_useTexture", 0);
    }
    m_shader->SetInt("u_texture", 0);
    
    m_quadVAO->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::CreateDefaultTexture() {
    // 1x1 white pixel (RGBA)
    // Bound when no texture is provided to satisfy OpenGL's requirement
    // that all sampler uniforms have valid textures
    uint8_t whitePixel[] = { 255, 255, 255, 255 };
    m_defaultTexture = std::make_unique<Texture2D>(whitePixel, 1, 1);
}

} // namespace engine

