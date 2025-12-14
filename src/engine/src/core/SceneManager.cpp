#include "engine/core/SceneManager.h"
#include "engine/core/Scene.h"
#include <SDL3/SDL_log.h>

namespace engine {

void SceneManager::PushScene(std::unique_ptr<Scene> scene) {
    if (m_pendingOp != PendingOp::None) {
        SDL_Log("SceneManager: Warning - overwriting pending transition");
    }
    m_pendingOp = PendingOp::Push;
    m_pendingScene = std::move(scene);
}

void SceneManager::PopScene() {
    if (m_pendingOp != PendingOp::None) {
        SDL_Log("SceneManager: Warning - overwriting pending transition");
    }
    m_pendingOp = PendingOp::Pop;
    m_pendingScene.reset();
}

void SceneManager::SwitchScene(std::unique_ptr<Scene> scene) {
    if (m_pendingOp != PendingOp::None) {
        SDL_Log("SceneManager: Warning - overwriting pending transition");
    }
    m_pendingOp = PendingOp::Switch;
    m_pendingScene = std::move(scene);
}

void SceneManager::Clear() {
    // Clear is immediate (used at shutdown)
    // First cancel any pending transition
    m_pendingOp = PendingOp::None;
    m_pendingScene.reset();
    
    // Exit all scenes in reverse order
    while (!m_scenes.empty()) {
        m_scenes.back()->OnExit();
        m_scenes.pop_back();
    }
}

void SceneManager::Update(float deltaTime, const Input& input) {
    // Process pending transitions first (handles initial scene push)
    ProcessPendingTransition();
    
    // Update current scene
    if (!m_scenes.empty()) {
        m_scenes.back()->Update(deltaTime, input);
    }
    
    // Process any transition requested during this update
    ProcessPendingTransition();
}

void SceneManager::Render(Renderer2D& renderer) {
    if (!m_scenes.empty()) {
        m_scenes.back()->Render(renderer);
    }
}

Scene* SceneManager::GetCurrentScene() {
    return m_scenes.empty() ? nullptr : m_scenes.back().get();
}

const Scene* SceneManager::GetCurrentScene() const {
    return m_scenes.empty() ? nullptr : m_scenes.back().get();
}

void SceneManager::ProcessPendingTransition() {
    if (m_pendingOp == PendingOp::None) return;
    
    switch (m_pendingOp) {
        case PendingOp::Push:
            ExecutePush(std::move(m_pendingScene));
            break;
        case PendingOp::Pop:
            ExecutePop();
            break;
        case PendingOp::Switch:
            ExecuteSwitch(std::move(m_pendingScene));
            break;
        default:
            break;
    }
    
    m_pendingOp = PendingOp::None;
}

void SceneManager::ExecutePush(std::unique_ptr<Scene> scene) {
    if (!m_scenes.empty()) {
        m_scenes.back()->OnPause();
    }
    m_scenes.push_back(std::move(scene));
    m_scenes.back()->OnEnter();
}

void SceneManager::ExecutePop() {
    if (m_scenes.empty()) return;
    
    m_scenes.back()->OnExit();
    m_scenes.pop_back();
    
    if (!m_scenes.empty()) {
        m_scenes.back()->OnResume();
    }
}

void SceneManager::ExecuteSwitch(std::unique_ptr<Scene> scene) {
    if (!m_scenes.empty()) {
        m_scenes.back()->OnExit();
        m_scenes.pop_back();
    }
    m_scenes.push_back(std::move(scene));
    m_scenes.back()->OnEnter();
}

} // namespace engine
