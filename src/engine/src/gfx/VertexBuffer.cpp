#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/GLFunctions.h"

namespace engine {

VertexBuffer::VertexBuffer(const void* data, size_t size)
    : VertexBuffer(data, size, false) {
}

VertexBuffer::VertexBuffer(const void* data, size_t size, bool dynamic) {
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
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

void VertexBuffer::SetData(const void* data, size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

} // namespace engine

