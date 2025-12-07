#include "engine/gfx/IndexBuffer.h"
#include "engine/gfx/GLFunctions.h"

namespace engine {

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
    : m_count(count) {
    glGenBuffers(1, &m_bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    if (m_bufferID != 0) {
        glDeleteBuffers(1, &m_bufferID);
    }
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace engine

