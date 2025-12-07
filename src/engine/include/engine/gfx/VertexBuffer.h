#pragma once

#include <SDL3/SDL_opengl.h>
#include <cstddef>

namespace engine {

/**
 * RAII wrapper for OpenGL Vertex Buffer Object (VBO)
 * Stores vertex data on the GPU
 */
class VertexBuffer {
public:
    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();
    
    // Non-copyable
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    
    void Bind() const;
    void Unbind() const;
    
    GLuint GetID() const { return m_bufferID; }

private:
    GLuint m_bufferID = 0;
};

} // namespace engine

