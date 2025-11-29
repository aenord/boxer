#include "engine/platform/Input.h"

namespace engine {

Input::Input() {
    // State arrays are zero-initialized
}

Input::~Input() {
}

void Input::Update() {
    // Copy current frame state to previous frame state
    // This enables "just pressed" detection by comparing current vs previous
    for (int i = 0; i < MAX_KEYS; ++i) {
        m_keysPrevious[i] = m_keys[i];
    }
    for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
        m_mouseButtonsPrevious[i] = m_mouseButtons[i];
    }
}

void Input::ProcessEvent(const SDL_Event& event) {
    // Update current input state based on SDL event type
    if (event.type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode key = event.key.key;
        if (key < MAX_KEYS) {
            m_keys[key] = true;
        }
    } else if (event.type == SDL_EVENT_KEY_UP) {
        SDL_Keycode key = event.key.key;
        if (key < MAX_KEYS) {
            m_keys[key] = false;
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        Uint8 button = event.button.button;
        if (button < MAX_MOUSE_BUTTONS) {
            m_mouseButtons[button] = true;
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        Uint8 button = event.button.button;
        if (button < MAX_MOUSE_BUTTONS) {
            m_mouseButtons[button] = false;
        }
    } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
        // Update mouse position
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;
    }
}

bool Input::IsKeyPressed(SDL_Keycode key) const {
    if (key < MAX_KEYS) {
        return m_keys[key];
    }
    return false;
}

bool Input::IsKeyJustPressed(SDL_Keycode key) const {
    if (key < MAX_KEYS) {
        return m_keys[key] && !m_keysPrevious[key];
    }
    return false;
}

bool Input::IsKeyJustReleased(SDL_Keycode key) const {
    if (key < MAX_KEYS) {
        return !m_keys[key] && m_keysPrevious[key];
    }
    return false;
}

bool Input::IsMouseButtonPressed(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return m_mouseButtons[button];
    }
    return false;
}

bool Input::IsMouseButtonJustPressed(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return m_mouseButtons[button] && !m_mouseButtonsPrevious[button];
    }
    return false;
}

bool Input::IsMouseButtonJustReleased(Uint8 button) const {
    if (button < MAX_MOUSE_BUTTONS) {
        return !m_mouseButtons[button] && m_mouseButtonsPrevious[button];
    }
    return false;
}

void Input::GetMousePosition(int* x, int* y) const {
    if (x) *x = m_mouseX;
    if (y) *y = m_mouseY;
}

} // namespace engine

