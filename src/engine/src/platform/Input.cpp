#include "engine/platform/Input.h"

namespace engine {

Input::Input() = default;
Input::~Input() = default;

void Input::Update(float /*deltaTime*/) {
    // Reset mouse delta (accumulated from motion events during this frame)
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;
    
    // Reset justPressed/justReleased flags for next frame
    // These flags are set in ProcessEvent and should only be true for one frame
    // Note: This runs AFTER game logic, so IsKeyJustPressed() works correctly
    for (auto& key : m_keys) {
        key.justPressed = false;
        key.justReleased = false;
    }
    
    for (auto& btn : m_mouseButtons) {
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
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            SDL_Scancode scancode = event.key.scancode;
            if (scancode < MAX_SCANCODES) {
                m_keys[scancode].pressed = false;
                m_keys[scancode].justReleased = true;
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

