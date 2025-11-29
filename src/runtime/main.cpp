#include "engine/core/Engine.h"
#include "engine/gfx/Camera2D.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_opengl.h>

// Test camera instance
static engine::Camera2D g_camera;

// Simple test: log key presses and move camera
void TestUpdate(float deltaTime, const engine::Input& input) {
    // Move camera with arrow keys
    float moveSpeed = 100.0f * deltaTime; // pixels per second
    engine::Vec2 moveDelta(0.0f, 0.0f);
    
    if (input.IsKeyPressed(SDL_SCANCODE_LEFT)) {
        moveDelta.x -= moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_RIGHT)) {
        moveDelta.x += moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_UP)) {
        moveDelta.y += moveSpeed;
    }
    if (input.IsKeyPressed(SDL_SCANCODE_DOWN)) {
        moveDelta.y -= moveSpeed;
    }
    
    if (moveDelta.x != 0.0f || moveDelta.y != 0.0f) {
        g_camera.Move(moveDelta);
        SDL_Log("Camera position: (%.2f, %.2f)", g_camera.position.x, g_camera.position.y);
    }
    
    // Log key presses
    if (input.IsKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
        SDL_Log("Escape key pressed");
    }
    if (input.IsKeyJustPressed(SDL_SCANCODE_SPACE)) {
        SDL_Log("Space key pressed");
    }
    
    // Test view-projection matrix
    if (input.IsKeyJustPressed(SDL_SCANCODE_P)) {
        engine::Mat4 vp = g_camera.GetViewProjectionMatrix();
        SDL_Log("View-Projection Matrix:");
        SDL_Log("  [%.3f, %.3f, %.3f, %.3f]", vp.m[0], vp.m[4], vp.m[8], vp.m[12]);
        SDL_Log("  [%.3f, %.3f, %.3f, %.3f]", vp.m[1], vp.m[5], vp.m[9], vp.m[13]);
        SDL_Log("  [%.3f, %.3f, %.3f, %.3f]", vp.m[2], vp.m[6], vp.m[10], vp.m[14]);
        SDL_Log("  [%.3f, %.3f, %.3f, %.3f]", vp.m[3], vp.m[7], vp.m[11], vp.m[15]);
    }
}

void TestRender() {
    // Simple clear with default background color
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
    engine::Engine engine("MyEngine", 800, 600);
    
    // Initialize camera
    g_camera.SetViewportSize(800.0f, 600.0f);
    g_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
    g_camera.zoom = 1.0f;
    
    SDL_Log("Camera2D initialized");
    SDL_Log("Controls:");
    SDL_Log("  Arrow Keys - Move camera");
    SDL_Log("  P - Print view-projection matrix");
    
    // Register callbacks for game logic
    engine.SetUpdateCallback(TestUpdate);
    engine.SetRenderCallback(TestRender);
    
    // Start game loop
    engine.Run();
    return 0;
}
