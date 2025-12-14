#pragma once

#include <SDL3/SDL_opengl.h>
#include <string>
#include <cstdint>

namespace engine {

/**
 * Texture filtering mode
 * Nearest: Pixel-perfect, sharp edges (best for pixel art)
 * Linear: Smooth interpolation (best for high-res textures)
 */
enum class TextureFilter {
    Nearest,  // GL_NEAREST - no interpolation
    Linear    // GL_LINEAR - bilinear interpolation
};

/**
 * 2D texture wrapper for OpenGL
 * Loads image files and manages GPU texture resources
 */
class Texture2D {
public:
    // Load texture from file path
    explicit Texture2D(const std::string& path, TextureFilter filter = TextureFilter::Linear);
    
    // Create texture from raw pixel data (RGBA format)
    Texture2D(const uint8_t* data, int width, int height, TextureFilter filter = TextureFilter::Linear);
    
    ~Texture2D();
    
    // Non-copyable
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    
    // Moveable
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;
    
    // Bind texture to a texture slot (0-15)
    void Bind(uint32_t slot = 0) const;
    static void Unbind();
    
    // Getters
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    GLuint GetID() const { return m_textureID; }
    bool IsValid() const { return m_textureID != 0; }

private:
    GLuint m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
    
    // Create texture from raw RGBA data
    void CreateFromData(const uint8_t* data, int width, int height, TextureFilter filter);
};

} // namespace engine

