#include "engine/gfx/GLContext.h"
#include <SDL3/SDL_opengl.h>

namespace engine {

GLContext::GLContext(SDL_Window* window) {
    // Configure OpenGL context attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create OpenGL context (automatically made current by SDL)
    m_context = SDL_GL_CreateContext(window);
    if (!m_context) {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
    }
}

GLContext::~GLContext() {
    // Cleanup: destroy OpenGL context
    if (m_context) {
        SDL_GL_DestroyContext(m_context);
    }
}

void GLContext::SwapBuffers(SDL_Window* window) {
    // Swap front and back buffers to display the rendered frame
    SDL_GL_SwapWindow(window);
}

} // namespace engine

