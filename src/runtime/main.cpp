#include "engine/core/Engine.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_opengl.h>

// Simple test: log key presses
void TestUpdate(float deltaTime, const engine::Input& input) {
    // Example: check for specific keys using scancodes and log when pressed
    if (input.IsKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
        SDL_Log("Escape key pressed");
    }
    if (input.IsKeyJustPressed(SDL_SCANCODE_SPACE)) {
        SDL_Log("Space key pressed");
    }
    
    (void)deltaTime; // Suppress unused parameter warning
}

void TestRender() {
    // Simple clear with default background color
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
    engine::Engine engine("MyEngine", 800, 600);
    
    // Register callbacks for game logic
    engine.SetUpdateCallback(TestUpdate);
    engine.SetRenderCallback(TestRender);
    
    // Start game loop
    engine.Run();
    return 0;
}
