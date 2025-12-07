#include "engine/gfx/VertexArray.h"
#include "engine/gfx/VertexBuffer.h"
#include "engine/gfx/IndexBuffer.h"
#include "engine/gfx/GLFunctions.h"

namespace engine {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_arrayID);
}

VertexArray::~VertexArray() {
    if (m_arrayID != 0) {
        glDeleteVertexArrays(1, &m_arrayID);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(m_arrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::SetQuadLayout() {
    // Standard 2D quad vertex layout:
    // Position: vec2 (location 0)
    // TexCoord: vec2 (location 1)
    // Color:    vec4 (location 2)
    // Total stride: 8 floats = 32 bytes per vertex
    
    const GLsizei stride = 8 * sizeof(float);
    
    // Position attribute (location 0): 2 floats at offset 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // TexCoord attribute (location 1): 2 floats at offset 8 bytes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Color attribute (location 2): 4 floats at offset 16 bytes
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void VertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> vbo) {
    m_vertexBuffer = vbo;
    Bind();
    vbo->Bind();
}

void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo) {
    m_indexBuffer = ibo;
    Bind();
    ibo->Bind();
}

} // namespace engine

