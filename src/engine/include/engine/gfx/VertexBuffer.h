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
    // Create static buffer with data
    VertexBuffer(const void* data, size_t size);
    
    // Create buffer (optionally dynamic for frequent updates)
    VertexBuffer(const void* data, size_t size, bool dynamic);
    
    ~VertexBuffer();
    
    // Non-copyable
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    
    void Bind() const;
    void Unbind() const;
    
    // Update buffer data (for dynamic buffers)
    void SetData(const void* data, size_t size);
    
    GLuint GetID() const { return m_bufferID; }

private:
    GLuint m_bufferID = 0;
};

} // namespace engine

