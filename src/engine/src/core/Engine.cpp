#include "engine/core/Engine.h"
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_timer.h>

namespace engine {

Engine::Engine(const char* title, int width, int height)
    : m_window(title, width, height)           // Initialize SDL and create window
    , m_glContext(m_window.GetWindow())        // Create OpenGL context from window
{
}

Engine::~Engine() {
    // Subsystems automatically cleaned up via destructors
}

void Engine::Run() {
    // Frame rate target: 60 FPS = 16.67ms per frame
    const Uint64 targetFrameTimeNS = SDL_NS_PER_SECOND / 60;
    Uint64 lastFrameTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    
    // Main game loop
    while (!m_window.ShouldClose()) {
        // Calculate delta time using high-resolution performance counter
        Uint64 currentTime = SDL_GetPerformanceCounter();
        Uint64 frameTimeNS = ((currentTime - lastFrameTime) * SDL_NS_PER_SECOND) / frequency;
        lastFrameTime = currentTime;
        float deltaTime = static_cast<float>(frameTimeNS) / SDL_NS_PER_SECOND;
        
        // Poll all SDL events and feed to input system
        SDL_Event event;
        while (m_window.PollEvent(&event)) {
            m_input.ProcessEvent(event);
        }
        
        // Update game logic first (check justPressed before it gets reset)
        Update(deltaTime);
        
        // Update input state: process timers and reset justPressed/justReleased flags for next frame
        m_input.Update(deltaTime);
        
        // Render frame
        Render();
        
        // Display rendered frame
        m_glContext.SwapBuffers(m_window.GetWindow());
        
        // Frame rate limiting: sleep if frame completed early
        Uint64 elapsedNS = ((SDL_GetPerformanceCounter() - currentTime) * SDL_NS_PER_SECOND) / frequency;
        if (elapsedNS < targetFrameTimeNS) {
            SDL_DelayNS(targetFrameTimeNS - elapsedNS);
        }
    }
}

void Engine::Update(float deltaTime) {
    // Call user-provided update callback if registered
    if (m_updateCallback) {
        m_updateCallback(deltaTime, m_input);
    }
}

void Engine::Render() {
    // Call user-provided render callback if registered, otherwise default clear
    if (m_renderCallback) {
        m_renderCallback();
    } else {
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

} // namespace engine

