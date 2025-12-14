#pragma once

#include <memory>
#include <vector>

namespace engine {

class Scene;
class Input;
class Renderer2D;

/**
 * Stack-based scene manager with deferred transitions.
 * Only the topmost scene receives Update/Render calls.
 * Transitions are deferred until frame end, safe to call from Update().
 */
class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;
    
    // Non-copyable
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    
    // Push scene onto stack (deferred, pauses current scene)
    void PushScene(std::unique_ptr<Scene> scene);
    
    // Pop current scene (deferred, resumes previous scene)
    void PopScene();
    
    // Replace current scene (deferred)
    void SwitchScene(std::unique_ptr<Scene> scene);
    
    // Clear all scenes immediately (for shutdown)
    void Clear();
    
    // Update current scene, then process pending transitions
    void Update(float deltaTime, const Input& input);
    
    // Render current scene
    void Render(Renderer2D& renderer);
    
    // Query scene stack
    bool HasScenes() const { return !m_scenes.empty(); }
    size_t GetSceneCount() const { return m_scenes.size(); }
    Scene* GetCurrentScene();
    const Scene* GetCurrentScene() const;

private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    
    // Pending transition (applied after Update completes)
    enum class PendingOp { None, Push, Pop, Switch };
    PendingOp m_pendingOp = PendingOp::None;
    std::unique_ptr<Scene> m_pendingScene;
    
    void ProcessPendingTransition();
    void ExecutePush(std::unique_ptr<Scene> scene);
    void ExecutePop();
    void ExecuteSwitch(std::unique_ptr<Scene> scene);
};

} // namespace engine
