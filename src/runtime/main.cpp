#include "engine/core/Engine.h"
#include "engine/core/Scene.h"
#include "engine/core/SceneManager.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/gfx/SpriteSheet.h"
#include "engine/gfx/Tilemap.h"
#include "engine/math/Vec2.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>

/**
 * Example scene demonstrating the Scene system.
 * Contains a tilemap with camera controls.
 */
class TilemapScene : public engine::Scene {
public:
    TilemapScene() = default;
    
    void OnEnter() override {
        SDL_Log("TilemapScene: OnEnter");
        
        // Initialize camera
        m_camera.SetViewportSize(800.0f, 600.0f);
        m_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
        m_camera.SetZoom(1.0f);
        m_camera.SetSmoothEnabled(true);
        m_camera.SetSmoothSpeed(5.0f);
        m_camera.SetPixelSnap(false);
        
        // Load tile sprite sheet
        if (!m_tileSheet.LoadFromFile("assets/tiles.json")) {
            SDL_Log("Failed to load tile sheet");
            return;
        }
        
        // Create tilemap (8x6 tiles, 64px each)
        m_tilemap = std::make_unique<engine::Tilemap>(8, 6, 64.0f);
        m_tilemap->SetSpriteSheet(&m_tileSheet);
        
        // Map tile indices to sprite names
        for (int i = 0; i < 8; ++i) {
            m_tilemap->SetTileSprite(i, "tile_" + std::to_string(i));
        }
        
        // Fill tilemap with a single tile type
        m_tilemap->Fill(0);
        
        SDL_Log("Map size: %dx%d tiles (%.0fx%.0f pixels)",
                m_tilemap->GetWidth(), m_tilemap->GetHeight(),
                m_tilemap->GetWorldWidth(), m_tilemap->GetWorldHeight());
        SDL_Log("Controls: WASD/Arrow keys to move camera");
    }
    
    void OnExit() override {
        SDL_Log("TilemapScene: OnExit");
        m_tilemap.reset();
    }
    
    void Update(float deltaTime, const engine::Input& input) override {
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
            m_camera.MoveTarget(moveDelta);
        }
        
        // Update camera (smooth follow)
        m_camera.Update(deltaTime);
    }
    
    void Render(engine::Renderer2D& renderer) override {
        renderer.BeginFrame(m_camera);
        
        // Draw tilemap centered in view
        if (m_tilemap) {
            engine::Vec2 offset(
                -m_tilemap->GetWorldWidth() * 0.5f,
                -m_tilemap->GetWorldHeight() * 0.5f
            );
            m_tilemap->Draw(renderer, offset);
        }
        
        renderer.EndFrame();
    }

private:
    engine::Camera2D m_camera;
    engine::SpriteSheet m_tileSheet;
    std::unique_ptr<engine::Tilemap> m_tilemap;
};

int main() {
    engine::Engine engine("Scene System Test", 800, 600);
    
    // Initialize renderer (must happen after Engine creates GL context)
    engine::Renderer2D renderer;
    if (!renderer.Init()) {
        SDL_Log("Failed to initialize Renderer2D");
        return 1;
    }
    
    // Create scene manager and push initial scene
    engine::SceneManager sceneManager;
    sceneManager.PushScene(std::make_unique<TilemapScene>());
    
    // Set callbacks to use SceneManager
    engine.SetUpdateCallback([&sceneManager](float dt, const engine::Input& input) {
        sceneManager.Update(dt, input);
    });
    
    engine.SetRenderCallback([&sceneManager, &renderer]() {
        sceneManager.Render(renderer);
    });
    
    SDL_Log("Scene System Test");
    SDL_Log("Using SceneManager with TilemapScene");
    
    // Run game loop
    engine.Run();
    
    // Clean up scenes before renderer is destroyed
    sceneManager.Clear();
    
    return 0;
}
