#include "engine/platform/Window.h"

namespace engine {

Window::Window(const char* title, int width, int height) {
    // Initialize SDL video subsystem
    SDL_Init(SDL_INIT_VIDEO);
    
    // Create window with OpenGL flag (required for OpenGL context)
    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
    if (!m_window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
    }
}

Window::~Window() {
    // Cleanup: destroy window and quit SDL
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}

bool Window::PollEvent(SDL_Event* event) {
    // Poll single SDL event
    if (SDL_PollEvent(event)) {
        // Check for window close event
        if (event->type == SDL_EVENT_QUIT) {
            m_shouldClose = true;
        }
        return true;
    }
    return false;
}

} // namespace engine

