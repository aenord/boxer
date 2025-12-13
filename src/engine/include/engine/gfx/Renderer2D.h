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
class Texture2D;

/**
 * 2D batch renderer
 * Manages a single quad mesh and issues draw calls with per-quad transforms
 * Supports solid colors, textures, and tinted textures
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
    
    // Draw a solid-colored quad
    void DrawQuad(const Vec2& position, const Vec2& size, const Vec4& color);
    
    // Draw a textured quad (optionally tinted)
    void DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture, 
                  const Vec4& tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Check if initialized
    bool IsInitialized() const { return m_initialized; }

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexArray> m_quadVAO;
    std::unique_ptr<VertexBuffer> m_quadVBO;
    std::unique_ptr<IndexBuffer> m_quadIBO;
    std::unique_ptr<Texture2D> m_defaultTexture;  // 1x1 white texture used when no texture is provided
    
    Mat4 m_viewProjection;
    bool m_initialized = false;
    
    // Internal draw implementation
    void DrawQuadInternal(const Vec2& position, const Vec2& size, const Vec4& color, 
                          const Texture2D* texture);
    
    // Create quad mesh
    void CreateQuadMesh();
    
    // Create shader
    void CreateShader();
    
    // Create default texture (1x1 white)
    void CreateDefaultTexture();
};

} // namespace engine

