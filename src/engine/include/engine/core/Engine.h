#pragma once

#include "engine/platform/Window.h"
#include "engine/platform/Input.h"
#include "engine/gfx/GLContext.h"

namespace engine {

/**
 * Main engine orchestrator - manages game loop and coordinates subsystems
 */
class Engine {
public:
    Engine(const char* title, int width, int height);
    ~Engine();

    // Start the main game loop (blocks until window closes)
    void Run();
    
    // Get input state for querying keyboard/mouse
    const Input& GetInput() const { return m_input; }
    
    // Register callbacks for game logic
    void SetUpdateCallback(void (*callback)(float deltaTime, const Input& input)) {
        m_updateCallback = callback;
    }
    void SetRenderCallback(void (*callback)()) {
        m_renderCallback = callback;
    }

private:
    void Update(float deltaTime);
    void Render();

    // Core subsystems
    Window m_window;        // SDL window and event polling
    Input m_input;          // Keyboard/mouse state management
    GLContext m_glContext;  // OpenGL rendering context
    
    // Game logic callbacks
    void (*m_updateCallback)(float deltaTime, const Input& input) = nullptr;
    void (*m_renderCallback)() = nullptr;
};

} // namespace engine

