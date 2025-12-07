#pragma once

#include <SDL3/SDL_opengl.h>
#include <memory>

namespace engine {

class VertexBuffer;
class IndexBuffer;

/**
 * RAII wrapper for OpenGL Vertex Array Object (VAO)
 * Manages vertex attribute configuration and buffer bindings
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
     */
    void SetQuadLayout();
    
    void SetVertexBuffer(std::shared_ptr<VertexBuffer> vbo);
    void SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo);
    
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }
    GLuint GetID() const { return m_arrayID; }

private:
    GLuint m_arrayID = 0;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
};

} // namespace engine

