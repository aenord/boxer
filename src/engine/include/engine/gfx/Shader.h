#pragma once

#include <SDL3/SDL_opengl.h>
#include <string>
#include <unordered_map>

namespace engine {

struct Mat4;
struct Vec4;
struct Vec2;

/**
 * OpenGL shader program wrapper
 * Manages vertex and fragment shader compilation and linking
 */
class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    
    // Non-copyable
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    // Moveable
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    
    // Bind/Unbind shader program
    void Bind() const;
    void Unbind() const;
    
    // Set uniform values
    void SetMat4(const std::string& name, const Mat4& mat) const;
    void SetVec4(const std::string& name, const Vec4& vec) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetVec2(const std::string& name, const Vec2& vec) const;
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    
    // Check if shader is valid
    bool IsValid() const { return m_programID != 0; }

private:
    GLuint m_programID = 0;
    
    // Uniform location cache (mutable for const SetUniform methods)
    mutable std::unordered_map<std::string, GLint> m_uniformCache;
    
    // Compile and link shaders
    bool Compile(const std::string& vertexSource, const std::string& fragmentSource);
    GLuint CompileShader(GLenum type, const std::string& source);
    bool CheckCompileErrors(GLuint shader, const std::string& type);
    GLint GetUniformLocation(const std::string& name) const;
};

} // namespace engine

