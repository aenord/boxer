#include "engine/platform/Input.h"

namespace engine {

Input::Input() = default;
Input::~Input() = default;

void Input::Update(float deltaTime) {
    // Reset mouse delta (calculated per frame from motion events)
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;
    
    // Update key justPressed/justReleased flags and repeat timers
    for (auto& key : m_keys) {
        // Update timers for key repeat functionality
        if (key.pressed) {
            if (key.delayTimer > 0.f) {
                // Count down initial delay before repeat starts
                key.delayTimer -= deltaTime;
            } else if (key.repeatTimer > 0.f) {
                // Count down repeat interval
                key.repeatTimer -= deltaTime;
            }
        } else {
            // Reset timers when key is released
            key.delayTimer = 0.f;
            key.repeatTimer = 0.f;
        }
        
        // Reset justPressed/justReleased flags for next frame
        // These flags are set to true in ProcessEvent when key state changes
        // They remain true for one frame (current frame), then get reset here
        // Note: This reset happens AFTER game logic has checked them (see Engine::Run() order)
        key.justPressed = false;
        key.justReleased = false;
    }
    
    // Update mouse button justPressed/justReleased flags
    for (auto& btn : m_mouseButtons) {
        // Reset justPressed/justReleased flags for next frame
        // These flags are set to true in ProcessEvent when button state changes
        // They remain true for one frame (current frame), then get reset here
        btn.justPressed = false;
        btn.justReleased = false;
    }
}

void Input::ProcessEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            SDL_Scancode scancode = event.key.scancode;
            if (scancode < MAX_SCANCODES && !m_keys[scancode].pressed) {
                m_keys[scancode].pressed = true;
                m_keys[scancode].justPressed = true;
                m_keys[scancode].delayTimer = 0.2f;   // 200ms initial delay before repeat
                m_keys[scancode].repeatTimer = 0.035f; // 35ms between repeats
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            SDL_Scancode scancode = event.key.scancode;
            if (scancode < MAX_SCANCODES) {
                m_keys[scancode].pressed = false;
                m_keys[scancode].justReleased = true;
                m_keys[scancode].delayTimer = 0.f;
                m_keys[scancode].repeatTimer = 0.f;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            Uint8 button = event.button.button;
            if (button < MAX_MOUSE_BUTTONS) {
                m_mouseButtons[button].pressed = true;
                m_mouseButtons[button].justPressed = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            Uint8 button = event.button.button;
            if (button < MAX_MOUSE_BUTTONS) {
                m_mouseButtons[button].pressed = false;
                m_mouseButtons[button].justReleased = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            // Calculate mouse delta (movement since last frame)
            m_mouseDeltaX = event.motion.x - m_mouseX;
            m_mouseDeltaY = event.motion.y - m_mouseY;
            
            // Update mouse position
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            break;
        }
        default:
            break;
    }
}


bool Input::IsKeyPressed(SDL_Scancode scancode) const {
    if (scancode < MAX_SCANCODES) {
        return m_keys[scancode].pressed;
    }
    return false;
}

bool Input::IsKeyJustPressed(SDL_Scancode scancode) const {
    if (scancode < MAX_SCANCODES) {
        return m_keys[scancode].justPressed;
    }
    return false;
}

bool Input::IsKeyJustReleased(SDL_Scancode scancode) const {
    if (scancode < MAX_SCANCODES) {
        return m_keys[scancode].justReleased;
    }
    return false;
}

bool Input::IsMouseButtonPressed(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return m_mouseButtons[button].pressed;
    }
    return false;
}

bool Input::IsMouseButtonJustPressed(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return m_mouseButtons[button].justPressed;
    }
    return false;
}

bool Input::IsMouseButtonJustReleased(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return m_mouseButtons[button].justReleased;
    }
    return false;
}

void Input::GetMousePosition(int* x, int* y) const {
    if (x) *x = m_mouseX;
    if (y) *y = m_mouseY;
}

void Input::GetMouseDelta(int* dx, int* dy) const {
    if (dx) *dx = m_mouseDeltaX;
    if (dy) *dy = m_mouseDeltaY;
}

} // namespace engine

