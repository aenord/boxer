#include "engine/core/Engine.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/Texture2D.h"
#include "engine/math/Vec2.h"
#include "engine/math/Vec4.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>
#include <vector>
#include <cstdint>

// Global instances
static engine::Camera2D g_camera;
static engine::Renderer2D g_renderer;
static std::unique_ptr<engine::Texture2D> g_checkerTexture;
static std::unique_ptr<engine::Texture2D> g_pngTexture;

// Create a procedural checkerboard texture for testing
std::unique_ptr<engine::Texture2D> CreateCheckerboardTexture(int size, int tileSize) {
    std::vector<uint8_t> pixels(size * size * 4);
    
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int idx = (y * size + x) * 4;
            bool white = ((x / tileSize) + (y / tileSize)) % 2 == 0;
            uint8_t color = white ? 255 : 100;
            pixels[idx + 0] = color;  // R
            pixels[idx + 1] = color;  // G
            pixels[idx + 2] = color;  // B
            pixels[idx + 3] = 255;    // A
        }
    }
    
    return std::make_unique<engine::Texture2D>(pixels.data(), size, size);
}

// Update: handle input and camera movement
void GameUpdate(float deltaTime, const engine::Input& input) {
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
}

// Render: draw scene using Renderer2D
void GameRender() {
    g_renderer.BeginFrame(g_camera);
    
    // Solid colored quads
    g_renderer.DrawQuad({-250.0f, 0.0f}, {60.0f, 60.0f}, {1.0f, 0.0f, 0.0f, 1.0f});   // Red
    g_renderer.DrawQuad({-250.0f, 80.0f}, {60.0f, 60.0f}, {0.0f, 1.0f, 0.0f, 1.0f});  // Green
    g_renderer.DrawQuad({-250.0f, -80.0f}, {60.0f, 60.0f}, {0.0f, 0.0f, 1.0f, 1.0f}); // Blue
    
    // Full texture
    if (g_checkerTexture && g_checkerTexture->IsValid()) {
        g_renderer.DrawQuad({-100.0f, 0.0f}, {120.0f, 120.0f}, *g_checkerTexture);
    }
    
    // Sub-UV renderer test: draw quarters of the checkerboard texture
    if (g_checkerTexture && g_checkerTexture->IsValid()) {
        // Top-left quarter: UV (0,0.5) to (0.5,1)
        g_renderer.DrawQuad({50.0f, 60.0f}, {60.0f, 60.0f}, *g_checkerTexture,
                            {0.0f, 0.5f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        
        // Top-right quarter: UV (0.5,0.5) to (1,1)
        g_renderer.DrawQuad({120.0f, 60.0f}, {60.0f, 60.0f}, *g_checkerTexture,
                            {0.5f, 0.5f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        
        // Bottom-left quarter: UV (0,0) to (0.5,0.5)
        g_renderer.DrawQuad({50.0f, -10.0f}, {60.0f, 60.0f}, *g_checkerTexture,
                            {0.0f, 0.0f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f});
        
        // Bottom-right quarter: UV (0.5,0) to (1,0.5), with red tint
        g_renderer.DrawQuad({120.0f, -10.0f}, {60.0f, 60.0f}, *g_checkerTexture,
                            {0.5f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.5f, 0.5f, 1.0f});
    }
    
    // PNG texture
    if (g_pngTexture && g_pngTexture->IsValid()) {
        float height = 180.0f;
        float aspect = static_cast<float>(g_pngTexture->GetWidth()) / g_pngTexture->GetHeight();
        float width = height * aspect;
        g_renderer.DrawQuad({280.0f, 0.0f}, {width, height}, *g_pngTexture);
    }
    
    g_renderer.EndFrame();
}

int main() {
    engine::Engine engine("Texture Test", 800, 600);
    
    // Initialize camera
    g_camera.SetViewportSize(800.0f, 600.0f);
    g_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
    g_camera.zoom = 1.0f;
    
    // Initialize renderer
    if (!g_renderer.Init()) {
        SDL_Log("Failed to initialize Renderer2D");
        return 1;
    }
    
    // Create procedural checkerboard texture
    g_checkerTexture = CreateCheckerboardTexture(64, 8);
    if (g_checkerTexture && g_checkerTexture->IsValid()) {
        SDL_Log("Created checkerboard texture");
    }
    
    // Load PNG texture from file (tests stb_image)
    g_pngTexture = std::make_unique<engine::Texture2D>("assets/test.png");
    if (!g_pngTexture || !g_pngTexture->IsValid()) {
        SDL_Log("Warning: Could not load assets/test.png");
    }
    
    SDL_Log("Sub-UV Rendering Test");
    SDL_Log("Controls: WASD/Arrow keys to move camera");
    SDL_Log("Shows: solid colors, full texture, sub-UV quarters, PNG texture");
    
    // Register callbacks
    engine.SetUpdateCallback(GameUpdate);
    engine.SetRenderCallback(GameRender);
    
    // Run game loop
    engine.Run();
    
    // Cleanup
    g_pngTexture.reset();
    g_checkerTexture.reset();
    
    return 0;
}
