#pragma once

#include <SDL3/SDL.h>

namespace engine {

/**
 * SDL window wrapper - manages window creation, event polling, and resize detection
 */
class Window {
public:
    Window(const char* title, int width, int height, bool resizable = true);
    ~Window();

    // Poll single SDL event (returns true if event available)
    bool PollEvent(SDL_Event* event);
    
    // Check if window should close (user clicked close button)
    bool ShouldClose() const { return m_shouldClose; }
    
    // Check if window was resized since last call (resets flag)
    bool WasResized();
    
    // Get current window dimensions
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    
    // Size constraints
    void SetMinSize(int minWidth, int minHeight);
    void SetMaxSize(int maxWidth, int maxHeight);
    void SetResizable(bool resizable);
    void LockSize();  // Lock to current size (sets min = max = current)
    
    // Get SDL window handle (for OpenGL context creation)
    SDL_Window* GetWindow() const { return m_window; }

private:
    SDL_Window* m_window = nullptr;
    int m_width = 0;
    int m_height = 0;
    bool m_shouldClose = false;
    bool m_wasResized = false;
};

} // namespace engine

