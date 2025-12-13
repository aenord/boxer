#include "engine/gfx/Texture2D.h"
#include "engine/gfx/GLFunctions.h"
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_surface.h>

namespace engine {

// Use global OpenGL functions for basic texture operations (available in GL 1.1)
// These are directly linked, not loaded via GetProcAddress

Texture2D::Texture2D(const std::string& path) {
    // Load BMP using SDL (built-in, no SDL_image needed)
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        SDL_Log("Failed to load texture '%s': %s", path.c_str(), SDL_GetError());
        return;
    }
    
    // Convert to RGBA format
    SDL_Surface* converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);
    
    if (!converted) {
        SDL_Log("Failed to convert texture format: %s", SDL_GetError());
        return;
    }
    
    CreateFromData(static_cast<const uint8_t*>(converted->pixels), 
                   converted->w, converted->h);
    
    SDL_DestroySurface(converted);
    
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

void Texture2D::CreateFromData(const uint8_t* data, int width, int height) {
    m_width = width;
    m_height = height;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    SDL_Log("Created texture ID=%u (%dx%d)", m_textureID, width, height);
}

void Texture2D::Bind(uint32_t slot) const {
    if (m_textureID != 0) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
}

void Texture2D::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace engine

