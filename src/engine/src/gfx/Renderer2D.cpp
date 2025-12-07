#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/Shader.h"
#include "engine/gfx/VertexArray.h"
#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/IndexBuffer.h"
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

out vec4 FragColor;

void main() {
    FragColor = u_color * v_color;
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
    
    m_initialized = true;
    SDL_Log("Renderer2D initialized");
    return true;
}

void Renderer2D::Shutdown() {
    m_shader.reset();
    m_quadVAO.reset();
    m_quadVBO.reset();
    m_quadIBO.reset();
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
    
    m_quadVAO = std::make_shared<VertexArray>();
    m_quadVBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    m_quadIBO = std::make_shared<IndexBuffer>(indices, 6);
    
    m_quadVAO->SetVertexBuffer(m_quadVBO);
    m_quadVAO->SetQuadLayout();
    m_quadVAO->SetIndexBuffer(m_quadIBO);
    m_quadVAO->Unbind();
}

void Renderer2D::BeginFrame(const Camera2D& camera) {
    m_viewProjection = camera.GetViewProjectionMatrix();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_shader->Bind();
    m_shader->SetMat4("u_viewproj", m_viewProjection);
}

void Renderer2D::EndFrame() {
    m_shader->Unbind();
}

void Renderer2D::DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color) {
    if (!m_initialized) return;
    
    // Build transform matrix: translate then scale
    // Transform = Translate * Scale
    Mat4 transform;
    
    // Scale (in local space)
    transform.m[0] = size.x;   // scale X
    transform.m[5] = size.y;   // scale Y
    transform.m[10] = 1.0f;    // scale Z
    transform.m[15] = 1.0f;
    
    // Translate (applied after scale in column-major)
    transform.m[12] = position.x;
    transform.m[13] = position.y;
    transform.m[14] = 0.0f;
    
    m_shader->SetMat4("u_transform", transform);
    m_shader->SetVec4("u_color", color);
    
    m_quadVAO->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

} // namespace engine

