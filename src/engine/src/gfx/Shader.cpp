#include "engine/gfx/Shader.h"
#include "engine/gfx/GLFunctions.h"
#include "engine/math/Mat4.h"
#include "engine/math/Vec4.h"
#include "engine/math/Vec2.h"
#include <SDL3/SDL_log.h>

namespace engine {

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    Compile(vertexSource, fragmentSource);
}

Shader::~Shader() {
    if (m_programID != 0 && glDeleteProgram) {
        glDeleteProgram(m_programID);
    }
}

Shader::Shader(Shader&& other) noexcept
    : m_programID(other.m_programID)
    , m_uniformCache(std::move(other.m_uniformCache)) {
    other.m_programID = 0;  // Prevent double-delete
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        // Clean up existing program
        if (m_programID != 0 && glDeleteProgram) {
            glDeleteProgram(m_programID);
        }
        // Transfer ownership
        m_programID = other.m_programID;
        m_uniformCache = std::move(other.m_uniformCache);
        other.m_programID = 0;
    }
    return *this;
}

bool Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource) {
    // Compile vertex shader
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Create shader program
    m_programID = glCreateProgram();
    if (m_programID == 0) {
        SDL_Log("Failed to create shader program");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Attach and link
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);
    
    // Check linking
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_programID, 1024, nullptr, infoLog);
        SDL_Log("Shader linking failed: %s", infoLog);
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
    
    // Cleanup individual shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return m_programID != 0;
}

GLuint Shader::CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        SDL_Log("Failed to create shader");
        return 0;
    }
    
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    if (!CheckCompileErrors(shader, (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT")) {
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

bool Shader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        SDL_Log("Shader compilation error (%s): %s", type.c_str(), infoLog);
        return false;
    }
    
    return true;
}

void Shader::Bind() const {
    if (m_programID != 0) {
        glUseProgram(m_programID);
    }
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string& name) const {
    if (m_programID == 0) return -1;
    
    // Check cache first
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }
    
    // Query GL and cache result
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    
    return location;
}


// Uniform Setters: (SetMat4, SetVec4, SetVec4, SetVec2, SetFloat, SetInt)
// These functions upload data from the CPU to shader uniform variables on the GPU.
// The shader must be bound (via Bind()) before calling these.
// Uniform locations are cached on first lookup to avoid repeated GL queries.
//
// For example:
//   shader.Bind();
//   shader.SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
//   shader.SetVec4("u_color", 1.0f, 0.0f, 0.0f, 1.0f);
//   // ... then perform draw calls

void Shader::SetMat4(const std::string& name, const Mat4& mat) const {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, mat.Data());
    }
}

void Shader::SetVec4(const std::string& name, const Vec4& vec) const {
    SetVec4(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

void Shader::SetVec2(const std::string& name, const Vec2& vec) const {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform2f(location, vec.x, vec.y);
    }
}

void Shader::SetFloat(const std::string& name, float value) const {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::SetInt(const std::string& name, int value) const {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

} // namespace engine
