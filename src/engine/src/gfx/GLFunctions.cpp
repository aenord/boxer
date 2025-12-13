#include "engine/gfx/GLFunctions.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_log.h>

namespace engine {

// Shader functions
GLuint (APIENTRY *glCreateShader)(GLenum type) = nullptr;
void (APIENTRY *glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) = nullptr;
void (APIENTRY *glCompileShader)(GLuint shader) = nullptr;
void (APIENTRY *glDeleteShader)(GLuint shader) = nullptr;
GLuint (APIENTRY *glCreateProgram)(void) = nullptr;
void (APIENTRY *glAttachShader)(GLuint program, GLuint shader) = nullptr;
void (APIENTRY *glLinkProgram)(GLuint program) = nullptr;
void (APIENTRY *glDeleteProgram)(GLuint program) = nullptr;
void (APIENTRY *glUseProgram)(GLuint program) = nullptr;
GLint (APIENTRY *glGetUniformLocation)(GLuint program, const GLchar* name) = nullptr;
void (APIENTRY *glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) = nullptr;
void (APIENTRY *glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = nullptr;
void (APIENTRY *glUniform2f)(GLint location, GLfloat x, GLfloat y) = nullptr;
void (APIENTRY *glUniform1f)(GLint location, GLfloat v0) = nullptr;
void (APIENTRY *glUniform1i)(GLint location, GLint v0) = nullptr;
void (APIENTRY *glGetShaderiv)(GLuint shader, GLenum pname, GLint* params) = nullptr;
void (APIENTRY *glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) = nullptr;
void (APIENTRY *glGetProgramiv)(GLuint program, GLenum pname, GLint* params) = nullptr;
void (APIENTRY *glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog) = nullptr;

// Buffer functions
void (APIENTRY *glGenVertexArrays)(GLsizei n, GLuint* arrays) = nullptr;
void (APIENTRY *glBindVertexArray)(GLuint array) = nullptr;
void (APIENTRY *glDeleteVertexArrays)(GLsizei n, const GLuint* arrays) = nullptr;
void (APIENTRY *glGenBuffers)(GLsizei n, GLuint* buffers) = nullptr;
void (APIENTRY *glBindBuffer)(GLenum target, GLuint buffer) = nullptr;
void (APIENTRY *glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage) = nullptr;
void (APIENTRY *glDeleteBuffers)(GLsizei n, const GLuint* buffers) = nullptr;
void (APIENTRY *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) = nullptr;
void (APIENTRY *glEnableVertexAttribArray)(GLuint index) = nullptr;

// Draw functions
void (APIENTRY *glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices) = nullptr;

// Texture functions
void (APIENTRY *glGenTextures)(GLsizei n, GLuint* textures) = nullptr;
void (APIENTRY *glDeleteTextures)(GLsizei n, const GLuint* textures) = nullptr;
void (APIENTRY *glBindTexture)(GLenum target, GLuint texture) = nullptr;
void (APIENTRY *glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels) = nullptr;
void (APIENTRY *glTexParameteri)(GLenum target, GLenum pname, GLint param) = nullptr;
void (APIENTRY *glActiveTexture)(GLenum texture) = nullptr;
void (APIENTRY *glGenerateMipmap)(GLenum target) = nullptr;

bool LoadGLFunctions() {
    static bool loaded = false;
    if (loaded) return true;
    
    // Shader functions
    glCreateShader = (decltype(glCreateShader))SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource = (decltype(glShaderSource))SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader = (decltype(glCompileShader))SDL_GL_GetProcAddress("glCompileShader");
    glDeleteShader = (decltype(glDeleteShader))SDL_GL_GetProcAddress("glDeleteShader");
    glCreateProgram = (decltype(glCreateProgram))SDL_GL_GetProcAddress("glCreateProgram");
    glAttachShader = (decltype(glAttachShader))SDL_GL_GetProcAddress("glAttachShader");
    glLinkProgram = (decltype(glLinkProgram))SDL_GL_GetProcAddress("glLinkProgram");
    glDeleteProgram = (decltype(glDeleteProgram))SDL_GL_GetProcAddress("glDeleteProgram");
    glUseProgram = (decltype(glUseProgram))SDL_GL_GetProcAddress("glUseProgram");
    glGetUniformLocation = (decltype(glGetUniformLocation))SDL_GL_GetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (decltype(glUniformMatrix4fv))SDL_GL_GetProcAddress("glUniformMatrix4fv");
    glUniform4f = (decltype(glUniform4f))SDL_GL_GetProcAddress("glUniform4f");
    glUniform2f = (decltype(glUniform2f))SDL_GL_GetProcAddress("glUniform2f");
    glUniform1f = (decltype(glUniform1f))SDL_GL_GetProcAddress("glUniform1f");
    glUniform1i = (decltype(glUniform1i))SDL_GL_GetProcAddress("glUniform1i");
    glGetShaderiv = (decltype(glGetShaderiv))SDL_GL_GetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (decltype(glGetShaderInfoLog))SDL_GL_GetProcAddress("glGetShaderInfoLog");
    glGetProgramiv = (decltype(glGetProgramiv))SDL_GL_GetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (decltype(glGetProgramInfoLog))SDL_GL_GetProcAddress("glGetProgramInfoLog");
    
    // Buffer functions
    glGenVertexArrays = (decltype(glGenVertexArrays))SDL_GL_GetProcAddress("glGenVertexArrays");
    glBindVertexArray = (decltype(glBindVertexArray))SDL_GL_GetProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (decltype(glDeleteVertexArrays))SDL_GL_GetProcAddress("glDeleteVertexArrays");
    glGenBuffers = (decltype(glGenBuffers))SDL_GL_GetProcAddress("glGenBuffers");
    glBindBuffer = (decltype(glBindBuffer))SDL_GL_GetProcAddress("glBindBuffer");
    glBufferData = (decltype(glBufferData))SDL_GL_GetProcAddress("glBufferData");
    glDeleteBuffers = (decltype(glDeleteBuffers))SDL_GL_GetProcAddress("glDeleteBuffers");
    glVertexAttribPointer = (decltype(glVertexAttribPointer))SDL_GL_GetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (decltype(glEnableVertexAttribArray))SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    
    // Draw functions
    glDrawElements = (decltype(glDrawElements))SDL_GL_GetProcAddress("glDrawElements");
    
    // Texture functions
    glGenTextures = (decltype(glGenTextures))SDL_GL_GetProcAddress("glGenTextures");
    glDeleteTextures = (decltype(glDeleteTextures))SDL_GL_GetProcAddress("glDeleteTextures");
    glBindTexture = (decltype(glBindTexture))SDL_GL_GetProcAddress("glBindTexture");
    glTexImage2D = (decltype(glTexImage2D))SDL_GL_GetProcAddress("glTexImage2D");
    glTexParameteri = (decltype(glTexParameteri))SDL_GL_GetProcAddress("glTexParameteri");
    glActiveTexture = (decltype(glActiveTexture))SDL_GL_GetProcAddress("glActiveTexture");
    glGenerateMipmap = (decltype(glGenerateMipmap))SDL_GL_GetProcAddress("glGenerateMipmap");
    
    // Verify critical functions loaded
    if (!glCreateShader || !glCreateProgram || !glGenVertexArrays || !glGenBuffers || 
        !glGenTextures || !glActiveTexture) {
        SDL_Log("Failed to load required OpenGL functions");
        return false;
    }
    
    loaded = true;
    SDL_Log("OpenGL functions loaded successfully");
    return true;
}

} // namespace engine

