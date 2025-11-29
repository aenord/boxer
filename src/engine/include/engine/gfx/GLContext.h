#pragma once

#include <SDL3/SDL.h>

namespace engine {

/**
 * OpenGL context manager - handles context creation and buffer swapping
 */
class GLContext {
public:
    GLContext(SDL_Window* window);
    ~GLContext();

    // Swap front and back buffers to display rendered frame
    void SwapBuffers(SDL_Window* window);

private:
    SDL_GLContext m_context = nullptr;  // OpenGL context handle
};

} // namespace engine

