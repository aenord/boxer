#pragma once

#include <SDL3/SDL.h>

namespace engine {

/**
 * SDL window wrapper - manages window creation and event polling
 */
class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();

    // Poll single SDL event (returns true if event available)
    bool PollEvent(SDL_Event* event);
    
    // Check if window should close (user clicked close button)
    bool ShouldClose() const { return m_shouldClose; }
    
    // Get SDL window handle (for OpenGL context creation)
    SDL_Window* GetWindow() const { return m_window; }

private:
    SDL_Window* m_window = nullptr;  // SDL window handle
    bool m_shouldClose = false;       // Close flag set by SDL_EVENT_QUIT
};

} // namespace engine

