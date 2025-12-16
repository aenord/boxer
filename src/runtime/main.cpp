#include "engine/core/Engine.h"
#include "engine/core/Scene.h"
#include "engine/core/SceneManager.h"
#include "engine/gfx/Camera2D.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/physics/Collision.h"
#include "engine/math/Vec2.h"
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_log.h>
#include <cmath>

/**
 * Test scene 
 * - Player moves with WASD/arrows
 */
class TestScene : public engine::Scene {
public:
    TestScene() = default;

    void OnEnter() override {
        SDL_Log("TestScene entered");

        // Initialize camera
        m_camera.SetViewportSize(800.0f, 600.0f);
        m_camera.SetPosition(engine::Vec2(0.0f, 0.0f));
        m_camera.SetZoom(1.0f);

        // Player setup
        m_playerPos = engine::Vec2(0.0f, 0.0f);
        m_playerSize = engine::Vec2(48.0f, 48.0f);

        // Create obstacle boxes
        m_obstacles.push_back(engine::AABB::FromCenter(engine::Vec2(-150.0f, 0.0f), engine::Vec2(32.0f, 32.0f)));
        m_obstacles.push_back(engine::AABB::FromCenter(engine::Vec2(150.0f, 0.0f), engine::Vec2(32.0f, 32.0f)));
        m_obstacles.push_back(engine::AABB::FromCenter(engine::Vec2(0.0f, 150.0f), engine::Vec2(64.0f, 32.0f)));
        m_obstacles.push_back(engine::AABB::FromCenter(engine::Vec2(0.0f, -150.0f), engine::Vec2(32.0f, 64.0f)));

        SDL_Log("Controls: WASD/Arrow keys to move player");
        SDL_Log("Player color: Blue=idle, Green=moving, Orange=colliding");
    }

    void OnExit() override {
        SDL_Log("TestScene exited");
    }

    void Update(float deltaTime, const engine::Input& input) override {
        // // Move camera target based on input
        // float moveSpeed = 200.0f * deltaTime;
        // engine::Vec2 moveDelta(0.0f, 0.0f);

        // Player movement
        engine::Vec2 moveDir(0.0f, 0.0f);
        float moveSpeed = 200.0f;

        if (input.IsKeyPressed(SDL_SCANCODE_LEFT) || input.IsKeyPressed(SDL_SCANCODE_A)) {
            moveDir.x -= 1.0f;
        }
        if (input.IsKeyPressed(SDL_SCANCODE_RIGHT) || input.IsKeyPressed(SDL_SCANCODE_D)) {
            moveDir.x += 1.0f;
        }
        if (input.IsKeyPressed(SDL_SCANCODE_UP) || input.IsKeyPressed(SDL_SCANCODE_W)) {
            moveDir.y += 1.0f;
        }
        if (input.IsKeyPressed(SDL_SCANCODE_DOWN) || input.IsKeyPressed(SDL_SCANCODE_S)) {
            moveDir.y -= 1.0f;
        }

        // Normalize diagonal movement
        float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        if (len > 0.0f) {
            moveDir.x /= len;
            moveDir.y /= len;
        }

        // Apply movement
        engine::Vec2 velocity = moveDir * moveSpeed * deltaTime;
        engine::Vec2 newPos = m_playerPos + velocity;
        engine::Vec2 halfSize = m_playerSize * 0.5f;

        // Check collisions and resolve with push-to-edge
        m_isColliding = false;

        for (const auto& obstacle : m_obstacles) {
            engine::AABB playerBox = engine::AABB::FromCenter(newPos, halfSize);
            engine::Vec2 separation = engine::Collision::GetSeparation(playerBox, obstacle);
            
            if (separation.x != 0.0f || separation.y != 0.0f) {
                m_isColliding = true;
                newPos = newPos + separation;
            }
        }

        m_playerPos = newPos;
        m_isMoving = (len > 0.0f);

        m_camera.Update(deltaTime);
    }

    void Render(engine::Renderer2D& renderer) override {
        renderer.BeginFrame(m_camera);

        // Draw obstacles (red boxes)
        engine::Vec4 obstacleColor(0.9f, 0.2f, 0.2f, 1.0f);
        for (const auto& obstacle : m_obstacles) {
            renderer.DrawQuad(obstacle.GetCenter(), obstacle.GetSize(), obstacleColor);
        }

        // Determine player color based on state
        engine::Vec4 playerColor;
        if (m_isColliding) {
            playerColor = engine::Vec4(1.0f, 0.5f, 0.0f, 1.0f);  // Orange: colliding
        } else if (m_isMoving) {
            playerColor = engine::Vec4(0.2f, 0.8f, 0.3f, 1.0f);  // Green: moving
        } else {
            playerColor = engine::Vec4(0.2f, 0.6f, 1.0f, 1.0f);  // Blue: idle
        }

        // Draw player
        renderer.DrawQuad(m_playerPos, m_playerSize, playerColor);

        // Draw player collision box outline (debug)
        engine::AABB playerBox = engine::AABB::FromCenter(m_playerPos, m_playerSize * 0.5f);
        DrawBoxOutline(renderer, playerBox, engine::Vec4(1.0f, 1.0f, 1.0f, 0.5f));

        renderer.EndFrame();
    }

private:
    engine::Camera2D m_camera;

    // Player state
    engine::Vec2 m_playerPos;
    engine::Vec2 m_playerSize;
    bool m_isMoving = false;
    bool m_isColliding = false;

    // Obstacles
    std::vector<engine::AABB> m_obstacles;

    // Helper to draw box outline
    void DrawBoxOutline(engine::Renderer2D& renderer, const engine::AABB& box, 
                        const engine::Vec4& color, float thickness = 2.0f) {
        engine::Vec2 size = box.GetSize();
        float halfThick = thickness * 0.5f;
        
        // Bottom
        renderer.DrawQuad(
            engine::Vec2(box.GetCenter().x, box.min.y + halfThick),
            engine::Vec2(size.x, thickness), color);
        // Top
        renderer.DrawQuad(
            engine::Vec2(box.GetCenter().x, box.max.y - halfThick),
            engine::Vec2(size.x, thickness), color);
        // Left
        renderer.DrawQuad(
            engine::Vec2(box.min.x + halfThick, box.GetCenter().y),
            engine::Vec2(thickness, size.y), color);
        // Right
        renderer.DrawQuad(
            engine::Vec2(box.max.x - halfThick, box.GetCenter().y),
            engine::Vec2(thickness, size.y), color);
    }
};

int main() {
    engine::Engine engine("Boxer Test Scene", 800, 600);

    // Initialize renderer
    engine::Renderer2D renderer;
    if (!renderer.Init()) {
        SDL_Log("Failed to initialize Renderer2D");
        return 1;
    }

    renderer.SetClearColor(0.15f, 0.15f, 0.2f, 1.0f);

    // Create scene manager and push test scene
    engine::SceneManager sceneManager;
    sceneManager.PushScene(std::make_unique<TestScene>());

    engine.SetUpdateCallback([&sceneManager](float dt, const engine::Input& input) {
        sceneManager.Update(dt, input);
    });

    engine.SetRenderCallback([&sceneManager, &renderer]() {
        sceneManager.Render(renderer);
    });

    engine.Run();
    sceneManager.Clear();

    return 0;
}
