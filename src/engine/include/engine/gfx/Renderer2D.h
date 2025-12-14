#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Vec4.h"
#include "engine/math/Mat4.h"
#include "engine/gfx/QuadVertex.h"
#include <memory>
#include <vector>
#include <array>
#include <cstdint>

namespace engine {

/**
 * Texture flip flags (can be combined with |)
 */
enum class Flip : uint8_t {
    None       = 0,
    Horizontal = 1 << 0,  // Flip on X axis (left-right)
    Vertical   = 1 << 1,  // Flip on Y axis (top-bottom)
    Both       = Horizontal | Vertical
};

// Allow bitwise operations on Flip
inline Flip operator|(Flip a, Flip b) {
    return static_cast<Flip>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline bool operator&(Flip a, Flip b) {
    return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Camera2D;
class Texture2D;

// Batch limits
constexpr uint32_t MAX_QUADS = 10000;
constexpr uint32_t MAX_VERTICES = MAX_QUADS * VERTICES_PER_QUAD;
constexpr uint32_t MAX_INDICES = MAX_QUADS * INDICES_PER_QUAD;
constexpr uint32_t MAX_TEXTURE_SLOTS = 16;  // OpenGL minimum guaranteed

/**
 * 2D batch renderer
 * Accumulates quads and renders them in batches for efficiency
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
    
    // Draw a rotated solid-colored quad
    void DrawQuad(const Vec2& position, const Vec2& size, float rotation, const Vec4& color);
    
    // Draw a textured quad (optionally tinted)
    void DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture, 
                  const Vec4& tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Draw a rotated textured quad
    void DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                  const Texture2D& texture, const Vec4& tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Draw a sub-region of a texture (for sprite sheets)
    // uvRect: (minU, minV, maxU, maxV) - normalized texture coordinates
    void DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture,
                  const Vec4& uvRect, const Vec4& tint);
    
    // Draw a rotated sub-region of a texture (full sprite control)
    // rotation: angle in radians
    void DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                  const Texture2D& texture, const Vec4& uvRect, const Vec4& tint);
    
    // Draw a flipped textured quad
    void DrawQuad(const Vec2& position, const Vec2& size, const Texture2D& texture,
                  Flip flip, const Vec4& tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Draw a rotated and flipped textured quad (full control)
    void DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                  const Texture2D& texture, Flip flip,
                  const Vec4& tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Draw a rotated, flipped sub-region (ultimate control for sprites)
    void DrawQuad(const Vec2& position, const Vec2& size, float rotation,
                  const Texture2D& texture, const Vec4& uvRect,
                  Flip flip, const Vec4& tint);
    
    // Check if initialized
    bool IsInitialized() const { return m_initialized; }
    
    // Set background clear color
    void SetClearColor(const Vec4& color) { m_clearColor = color; }
    void SetClearColor(float r, float g, float b, float a = 1.0f) { 
        m_clearColor = Vec4(r, g, b, a); 
    }
    Vec4 GetClearColor() const { return m_clearColor; }

private:
    // OpenGL resources
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexArray> m_quadVAO;
    std::unique_ptr<VertexBuffer> m_quadVBO;
    std::unique_ptr<IndexBuffer> m_quadIBO;
    std::unique_ptr<Texture2D> m_defaultTexture;  // 1x1 white texture for solid colors
    
    // Batch state
    std::vector<QuadVertex> m_vertices;           // CPU-side vertex buffer
    uint32_t m_indexCount = 0;                    // Number of indices to draw
    
    // Texture slots for current batch
    std::array<const Texture2D*, MAX_TEXTURE_SLOTS> m_textureSlots;
    uint32_t m_textureSlotIndex = 1;              // 0 is reserved for default texture
    
    Mat4 m_viewProjection;
    Vec4 m_clearColor = Vec4(0.1f, 0.1f, 0.1f, 1.0f);  // Default dark gray
    bool m_initialized = false;
    
    // Flush accumulated quads to GPU
    void Flush();
    
    // Start a new batch
    void StartBatch();
    
    // Internal: add a quad to the batch
    // uvRect: (minU, minV, maxU, maxV) - use (0,0,1,1) for full texture
    // rotation: angle in radians (0 = no rotation)
    // flip: Flip flags for horizontal/vertical flipping
    void AddQuadToBatch(const Vec2& position, const Vec2& size, float rotation,
                        const Vec4& color, const Texture2D* texture, const Vec4& uvRect,
                        Flip flip);
    
    // Initialization helpers
    void CreateQuadMesh();
    void CreateShader();
    void CreateDefaultTexture();
};

} // namespace engine

