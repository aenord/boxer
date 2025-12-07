#pragma once

#include <SDL3/SDL_opengl.h>
#include <cstdint>

namespace engine {

/**
 * RAII wrapper for OpenGL Element Buffer Object (EBO/IBO)
 * Stores index data for indexed drawing
 */
class IndexBuffer {
public:
    IndexBuffer(const uint32_t* indices, uint32_t count);
    ~IndexBuffer();
    
    // Non-copyable
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    
    void Bind() const;
    void Unbind() const;
    
    uint32_t GetCount() const { return m_count; }
    GLuint GetID() const { return m_bufferID; }

private:
    GLuint m_bufferID = 0;
    uint32_t m_count = 0;
};

} // namespace engine

