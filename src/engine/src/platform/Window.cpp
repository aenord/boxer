#include "engine/platform/Window.h"

namespace engine {

Window::Window(const char* title, int width, int height, bool resizable)
    : m_width(width)
    , m_height(height)
{
    // Initialize SDL video subsystem
    SDL_Init(SDL_INIT_VIDEO);
    
    // Create window with OpenGL flag (resizable is optional)
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
    if (resizable) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    
    m_window = SDL_CreateWindow(title, width, height, flags);
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
    if (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_EVENT_QUIT:
                m_shouldClose = true;
                break;
                
            case SDL_EVENT_WINDOW_RESIZED:
                m_width = event->window.data1;
                m_height = event->window.data2;
                m_wasResized = true;
                break;
                
            default:
                break;
        }
        return true;
    }
    return false;
}

bool Window::WasResized() {
    bool resized = m_wasResized;
    m_wasResized = false;  // Reset flag after checking
    return resized;
}

void Window::SetMinSize(int minWidth, int minHeight) {
    if (m_window) {
        SDL_SetWindowMinimumSize(m_window, minWidth, minHeight);
    }
}

void Window::SetMaxSize(int maxWidth, int maxHeight) {
    if (m_window) {
        SDL_SetWindowMaximumSize(m_window, maxWidth, maxHeight);
    }
}

void Window::SetResizable(bool resizable) {
    if (m_window) {
        SDL_SetWindowResizable(m_window, resizable);
    }
}

void Window::LockSize() {
    // Lock window to current dimensions by setting min = max = current
    SetMinSize(m_width, m_height);
    SetMaxSize(m_width, m_height);
}

} // namespace engine

