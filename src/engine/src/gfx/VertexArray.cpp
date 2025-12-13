#include "engine/gfx/VertexArray.h"
#include "engine/gfx/GLFunctions.h"

namespace engine {

// Create a Vertex Array Object (VAO)
// VAOs store vertex attribute configurations for quick switching
// between different vertex layouts without reconfiguring each time
VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_arrayID);
}

VertexArray::~VertexArray() {
    if (m_arrayID != 0) {
        glDeleteVertexArrays(1, &m_arrayID);
    }
}

// Bind this VAO. All subsequent vertex attribute calls affect this VAO.
void VertexArray::Bind() const {
    glBindVertexArray(m_arrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

// Configure vertex attributes for 2D quad rendering
// Must be called while VAO is bound and VBO contains data
//
// Vertex layout (matches QuadVertex struct):
//   Location 0: position (vec2) - 8 bytes
//   Location 1: texCoord (vec2) - 8 bytes
//   Location 2: color    (vec4) - 16 bytes
//   Total stride: 32 bytes per vertex
void VertexArray::SetQuadLayout() {
    const GLsizei stride = 8 * sizeof(float);  // 32 bytes
    
    // Position: 2 floats at offset 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // TexCoord: 2 floats at offset 8 bytes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Color: 4 floats at offset 16 bytes
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

} // namespace engine

