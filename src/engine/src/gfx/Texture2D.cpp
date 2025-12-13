#include "engine/gfx/Texture2D.h"
#include "engine/gfx/GLFunctions.h"
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_log.h>
#include <stb_image.h>

namespace engine {

Texture2D::Texture2D(const std::string& path) {
    // Load image using stb_image (supports PNG, JPG, BMP, etc.)
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);  // OpenGL expects bottom-left origin
    uint8_t* data = stbi_load(path.c_str(), &width, &height, &channels, 4);  // Force RGBA
    
    if (!data) {
        SDL_Log("Failed to load texture '%s': %s", path.c_str(), stbi_failure_reason());
        return;
    }
    
    CreateFromData(data, width, height);
    stbi_image_free(data);
    
    if (m_textureID != 0) {
        SDL_Log("Loaded texture '%s' (%dx%d)", path.c_str(), m_width, m_height);
    }
}

Texture2D::Texture2D(const uint8_t* data, int width, int height) {
    CreateFromData(data, width, height);
}

Texture2D::~Texture2D() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_textureID(other.m_textureID)
    , m_width(other.m_width)
    , m_height(other.m_height) {
    other.m_textureID = 0;
    other.m_width = 0;
    other.m_height = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
    if (this != &other) {
        // Clean up existing texture
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }
        // Transfer ownership
        m_textureID = other.m_textureID;
        m_width = other.m_width;
        m_height = other.m_height;
        other.m_textureID = 0;
        other.m_width = 0;
        other.m_height = 0;
    }
    return *this;
}

// Upload RGBA pixel data to GPU and create OpenGL texture
// data: RGBA pixel data (4 bytes per pixel)
// width/height: image dimensions in pixels
void Texture2D::CreateFromData(const uint8_t* data, int width, int height) {
    m_width = width;
    m_height = height;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Texture wrapping: clamp to edge prevents sampling outside texture bounds
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Texture filtering: linear for smooth scaling (use GL_NEAREST for pixel art)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload pixel data to GPU
    // GL_RGBA: internal format and source format
    // GL_UNSIGNED_BYTE: 8 bits per channel
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    SDL_Log("Created texture ID=%u (%dx%d)", m_textureID, width, height);
}

// Bind texture to a texture slot for sampling in shaders
// OpenGL guarantees at least 16 slots (GL_TEXTURE0 through GL_TEXTURE15)
void Texture2D::Bind(uint32_t slot) const {
    if (m_textureID != 0) {
        glActiveTexture(GL_TEXTURE0 + slot);  // Select texture unit
        glBindTexture(GL_TEXTURE_2D, m_textureID);  // Bind texture to unit
    }
}

void Texture2D::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace engine

