#include "engine/core/Engine.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/math/Vec2.h"
#include "engine/math/Vec4.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>

// Global instances
static engine::Camera2D g_camera;
static engine::Renderer2D g_renderer;

// Update: handle input and camera movement
void GameUpdate(float deltaTime, const engine::Input& input) {
    // Move camera with arrow keys
    float moveSpeed = 200.0f * deltaTime;
    engine::Vec2 moveDelta(0.0f, 0.0f);
    
    if (input.IsKeyPressed(SDL_SCANCODE_LEFT) || input.IsKeyPressed(SDL_SCANCODE_A)) {
        moveDelta.x -= moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_RIGHT) || input.IsKeyPressed(SDL_SCANCODE_D)) {
        moveDelta.x += moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_UP) || input.IsKeyPressed(SDL_SCANCODE_W)) {
        moveDelta.y += moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_DOWN) || input.IsKeyPressed(SDL_SCANCODE_S)) {
        moveDelta.y -= moveSpeed;
    }
    
    if (moveDelta.x != 0.0f || moveDelta.y != 0.0f) {
        g_camera.Move(moveDelta);
    }
    
    // Log key presses
    if (input.IsKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
        SDL_Log("Escape pressed");
    }
}

// Render: draw scene using Renderer2D
void GameRender() {
    g_renderer.BeginFrame(g_camera);
    
    // Draw some colored quads
    g_renderer.DrawQuad({0.0f, 0.0f}, {100.0f, 100.0f}, {1.0f, 0.0f, 0.0f, 1.0f});      // Red center
    g_renderer.DrawQuad({150.0f, 0.0f}, {80.0f, 80.0f}, {0.0f, 1.0f, 0.0f, 1.0f});     // Green right
    g_renderer.DrawQuad({-150.0f, 0.0f}, {80.0f, 80.0f}, {0.0f, 0.0f, 1.0f, 1.0f});    // Blue left
    g_renderer.DrawQuad({0.0f, 120.0f}, {60.0f, 60.0f}, {1.0f, 1.0f, 0.0f, 1.0f});     // Yellow top
    g_renderer.DrawQuad({0.0f, -120.0f}, {60.0f, 60.0f}, {1.0f, 0.0f, 1.0f, 1.0f});    // Magenta bottom
    
    g_renderer.EndFrame();
}

int main() {
    engine::Engine engine("Renderer2D Test", 800, 600);
    
    // Initialize camera
    g_camera.SetViewportSize(800.0f, 600.0f);
    g_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
    g_camera.zoom = 1.0f;
    
    // Initialize renderer
    if (!g_renderer.Init()) {
        SDL_Log("Failed to initialize Renderer2D");
        return 1;
    }
    
    SDL_Log("Renderer2D Test");
    SDL_Log("Controls: WASD/Arrow keys to move camera");
    
    // Register callbacks
    engine.SetUpdateCallback(GameUpdate);
    engine.SetRenderCallback(GameRender);
    
    // Run game loop
    engine.Run();
    return 0;
}
