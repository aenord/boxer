#pragma once

#include <SDL3/SDL_opengl.h>

namespace engine {

/**
 * RAII wrapper for OpenGL Vertex Array Object (VAO)
 * Manages vertex attribute configuration
 */
class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    
    // Non-copyable
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    
    void Bind() const;
    void Unbind() const;
    
    /**
     * Configure vertex attributes for bound VBO
     * Standard 2D quad layout: position (vec2), texcoord (vec2), color (vec4)
     * Call this while VAO and VBO are bound
     */
    void SetQuadLayout();
    
    GLuint GetID() const { return m_arrayID; }

private:
    GLuint m_arrayID = 0;
};

} // namespace engine

