#pragma once

#include "engine/platform/Window.h"
#include "engine/platform/Input.h"
#include "engine/gfx/GLContext.h"
#include <functional>

namespace engine {

/**
 * Main engine orchestrator - manages game loop and coordinates subsystems
 */
class Engine {
public:
    // Callback types (allows lambdas, member functions, captures)
    using UpdateCallback = std::function<void(float deltaTime, const Input& input)>;
    using RenderCallback = std::function<void()>;

    Engine(const char* title, int width, int height);
    ~Engine();

    // Start the main game loop (blocks until window closes)
    void Run();
    
    // Get input state for querying keyboard/mouse
    const Input& GetInput() const { return m_input; }
    
    // Register callbacks for game logic
    void SetUpdateCallback(UpdateCallback callback) {
        m_updateCallback = std::move(callback);
    }
    void SetRenderCallback(RenderCallback callback) {
        m_renderCallback = std::move(callback);
    }

private:
    void Update(float deltaTime);
    void Render();

    // Core subsystems
    Window m_window;        // SDL window and event polling
    Input m_input;          // Keyboard/mouse state management
    GLContext m_glContext;  // OpenGL rendering context
    
    // Game logic callbacks
    UpdateCallback m_updateCallback;
    RenderCallback m_renderCallback;
};

} // namespace engine

