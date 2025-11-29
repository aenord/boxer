#pragma once

#include <SDL3/SDL.h>

namespace engine {

/**
 * Input state manager - tracks keyboard and mouse state with frame-to-frame comparison
 * for detecting "just pressed" and "just released" events
 */
class Input {
public:
    Input();
    ~Input();
    
    // Copy current state to previous state (called after game logic update)
    void Update();
    
    // Process SDL event to update current input state
    void ProcessEvent(const SDL_Event& event);
    
    // Keyboard state queries
    bool IsKeyPressed(SDL_Keycode key) const;        // Key currently held down
    bool IsKeyJustPressed(SDL_Keycode key) const;    // Key pressed this frame only
    bool IsKeyJustReleased(SDL_Keycode key) const;   // Key released this frame only
    
    // Mouse state queries
    bool IsMouseButtonPressed(Uint8 button) const;
    bool IsMouseButtonJustPressed(Uint8 button) const;
    bool IsMouseButtonJustReleased(Uint8 button) const;
    
    // Get current mouse position
    void GetMousePosition(int* x, int* y) const;
    
private:
    static constexpr int MAX_KEYS = 512;          // Maximum keycode value to track
    static constexpr int MAX_MOUSE_BUTTONS = 8;   // Maximum mouse button ID
    
    // Keyboard state: current and previous frame
    bool m_keys[MAX_KEYS] = {false};
    bool m_keysPrevious[MAX_KEYS] = {false};
    
    // Mouse state: current and previous frame
    bool m_mouseButtons[MAX_MOUSE_BUTTONS] = {false};
    bool m_mouseButtonsPrevious[MAX_MOUSE_BUTTONS] = {false};
    
    // Mouse position
    int m_mouseX = 0;
    int m_mouseY = 0;
};

} // namespace engine

