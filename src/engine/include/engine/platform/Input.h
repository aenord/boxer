#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>

namespace engine {

/**
 * Input state manager - tracks keyboard and mouse state per frame.
 * Provides pressed, justPressed, and justReleased queries.
 * Mouse delta tracking for relative movement.
 */
class Input {
public:
    Input();
    ~Input();
    
    // Update input state: reset justPressed/justReleased flags and mouse delta
    void Update(float deltaTime);
    
    // Process SDL event to update current input state
    void ProcessEvent(const SDL_Event& event);
    
    // Keyboard state queries (using scancodes for physical key detection)
    bool IsKeyPressed(SDL_Scancode scancode) const;
    bool IsKeyJustPressed(SDL_Scancode scancode) const;
    bool IsKeyJustReleased(SDL_Scancode scancode) const;
    
    // Mouse state queries
    bool IsMouseButtonPressed(Uint8 button) const;
    bool IsMouseButtonJustPressed(Uint8 button) const;
    bool IsMouseButtonJustReleased(Uint8 button) const;
    
    // Mouse position and movement
    void GetMousePosition(int* x, int* y) const;
    void GetMouseDelta(int* dx, int* dy) const;
    
private:
    // Key state structure for tracking press state
    struct KeyState {
        bool pressed = false;
        bool justPressed = false;
        bool justReleased = false;
    };
    
    // Mouse button state structure
    struct MouseButtonState {
        bool pressed = false;
        bool justPressed = false;
        bool justReleased = false;
    };
    
    static constexpr int MAX_SCANCODES = SDL_SCANCODE_COUNT;  // SDL3 scancode count (512)
    static constexpr int MAX_MOUSE_BUTTONS = 8;              // Maximum mouse button ID
    
    // Keyboard state array (indexed by scancode)
    KeyState m_keys[MAX_SCANCODES];
    
    // Mouse button state array
    MouseButtonState m_mouseButtons[MAX_MOUSE_BUTTONS];
    
    // Mouse position and delta
    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_mouseDeltaX = 0;
    int m_mouseDeltaY = 0;
};

} // namespace engine

