#include "engine/core/Engine.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/SpriteSheet.h"
#include "engine/gfx/Tilemap.h"
#include "engine/math/Vec2.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>

// Global instances
static engine::Camera2D g_camera;
static engine::Renderer2D g_renderer;
static engine::SpriteSheet g_tileSheet;
static std::unique_ptr<engine::Tilemap> g_tilemap;

// Update: handle input and camera movement
void GameUpdate(float deltaTime, const engine::Input& input) {
    // Move camera target based on input
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
        g_camera.MoveTarget(moveDelta);
    }
    
    // Update camera (smooth follow)
    g_camera.Update(deltaTime);
}

// Render: draw tilemap
void GameRender() {
    g_renderer.BeginFrame(g_camera);
    
    // Draw tilemap centered in view
    if (g_tilemap) {
        engine::Vec2 offset(
            -g_tilemap->GetWorldWidth() * 0.5f,
            -g_tilemap->GetWorldHeight() * 0.5f
        );
        g_tilemap->Draw(g_renderer, offset);
    }
    
    g_renderer.EndFrame();
}

int main() {
    engine::Engine engine("Tilemap Test", 800, 600);
    
    // Initialize camera with smooth following
    g_camera.SetViewportSize(800.0f, 600.0f);
    g_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
    g_camera.SetZoom(1.0f);
    g_camera.SetSmoothEnabled(true);
    g_camera.SetSmoothSpeed(5.0f);   // Adjust for faster/slower follow
    g_camera.SetPixelSnap(false);    // Set to true for crisp pixel art rendering
    
    // Initialize renderer
    if (!g_renderer.Init()) {
        SDL_Log("Failed to initialize Renderer2D");
        return 1;
    }
    
    // Load tile sprite sheet
    if (!g_tileSheet.LoadFromFile("assets/tiles.json")) {
        SDL_Log("Failed to load tile sheet");
        return 1;
    }
    
    // Create tilemap (8x6 tiles, 64px each)
    g_tilemap = std::make_unique<engine::Tilemap>(8, 6, 64.0f);
    g_tilemap->SetSpriteSheet(&g_tileSheet);
    
    // Map tile indices to sprite names
    for (int i = 0; i < 8; ++i) {
        g_tilemap->SetTileSprite(i, "tile_" + std::to_string(i));
    }
    
    // Fill tilemap with a single tile type
    g_tilemap->Fill(0);
    
    SDL_Log("Camera + Tilemap Test");
    SDL_Log("Map size: %dx%d tiles (%.0fx%.0f pixels)",
            g_tilemap->GetWidth(), g_tilemap->GetHeight(),
            g_tilemap->GetWorldWidth(), g_tilemap->GetWorldHeight());
    SDL_Log("Camera: smooth=%.1f, pixelSnap=%s", 
            g_camera.GetSmoothSpeed(), g_camera.IsPixelSnapEnabled() ? "on" : "off");
    SDL_Log("Controls: WASD/Arrow keys to move camera");
    
    // Register callbacks
    engine.SetUpdateCallback(GameUpdate);
    engine.SetRenderCallback(GameRender);
    
    // Run game loop
    engine.Run();
    
    // Cleanup
    g_tilemap.reset();
    
    return 0;
}
