#pragma once

#include <SDL3/SDL_opengl.h>

namespace engine {

/**
 * Centralized OpenGL function loading
 * Call LoadGLFunctions() once after creating OpenGL context
 */
bool LoadGLFunctions();

// Shader functions
extern GLuint (APIENTRY *glCreateShader)(GLenum type);
extern void (APIENTRY *glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
extern void (APIENTRY *glCompileShader)(GLuint shader);
extern void (APIENTRY *glDeleteShader)(GLuint shader);
extern GLuint (APIENTRY *glCreateProgram)(void);
extern void (APIENTRY *glAttachShader)(GLuint program, GLuint shader);
extern void (APIENTRY *glLinkProgram)(GLuint program);
extern void (APIENTRY *glDeleteProgram)(GLuint program);
extern void (APIENTRY *glUseProgram)(GLuint program);
extern GLint (APIENTRY *glGetUniformLocation)(GLuint program, const GLchar* name);
extern void (APIENTRY *glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void (APIENTRY *glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *glUniform2f)(GLint location, GLfloat x, GLfloat y);
extern void (APIENTRY *glUniform1f)(GLint location, GLfloat v0);
extern void (APIENTRY *glUniform1i)(GLint location, GLint v0);
extern void (APIENTRY *glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
extern void (APIENTRY *glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
extern void (APIENTRY *glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
extern void (APIENTRY *glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

// Buffer functions
extern void (APIENTRY *glGenVertexArrays)(GLsizei n, GLuint* arrays);
extern void (APIENTRY *glBindVertexArray)(GLuint array);
extern void (APIENTRY *glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
extern void (APIENTRY *glGenBuffers)(GLsizei n, GLuint* buffers);
extern void (APIENTRY *glBindBuffer)(GLenum target, GLuint buffer);
extern void (APIENTRY *glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
extern void (APIENTRY *glDeleteBuffers)(GLsizei n, const GLuint* buffers);
extern void (APIENTRY *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
extern void (APIENTRY *glEnableVertexAttribArray)(GLuint index);

// Draw functions
extern void (APIENTRY *glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);

} // namespace engine

