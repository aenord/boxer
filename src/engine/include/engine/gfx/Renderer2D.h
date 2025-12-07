#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Vec4.h"
#include "engine/math/Mat4.h"
#include <memory>

namespace engine {

class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Camera2D;

/**
 * 2D batch renderer
 * Manages a single quad mesh and issues draw calls with per-quad transforms
 */
class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();
    
    // Non-copyable
    Renderer2D(const Renderer2D&) = delete;
    Renderer2D& operator=(const Renderer2D&) = delete;
    
    // Initialize renderer (call after OpenGL context is created)
    bool Init();
    void Shutdown();
    
    // Begin/End frame
    void BeginFrame(const Camera2D& camera);
    void EndFrame();
    
    // Draw a colored quad
    void DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color);
    
    // Check if initialized
    bool IsInitialized() const { return m_initialized; }

private:
    std::unique_ptr<Shader> m_shader;
    std::shared_ptr<VertexArray> m_quadVAO;
    std::shared_ptr<VertexBuffer> m_quadVBO;
    std::shared_ptr<IndexBuffer> m_quadIBO;
    
    Mat4 m_viewProjection;
    bool m_initialized = false;
    
    // Create quad mesh
    void CreateQuadMesh();
    
    // Create shader
    void CreateShader();
};

} // namespace engine

