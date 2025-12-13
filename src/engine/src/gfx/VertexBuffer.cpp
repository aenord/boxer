#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/GLFunctions.h"

namespace engine {

// Convenience constructor for static buffers (data uploaded once)
VertexBuffer::VertexBuffer(const void* data, size_t size)
    : VertexBuffer(data, size, false) {
}

// Create a vertex buffer and upload initial data
// data: vertex data (can be nullptr for dynamic buffers)
// size: size in bytes
// dynamic: if true, uses GL_DYNAMIC_DRAW for frequent updates
VertexBuffer::VertexBuffer(const void* data, size_t size, bool dynamic) {
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    // GL_STATIC_DRAW: data set once, used many times
    // GL_DYNAMIC_DRAW: data changed frequently, used many times
    glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    if (m_bufferID != 0) {
        glDeleteBuffers(1, &m_bufferID);
    }
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Update buffer contents (for dynamic buffers that change each frame)
// Buffer should have been created with dynamic=true for best performance
// size must not exceed original allocation
void VertexBuffer::SetData(const void* data, size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    // glBufferSubData updates existing buffer without reallocating
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

} // namespace engine

