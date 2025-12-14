#pragma once

namespace engine {

class Input;
class Renderer2D;

/**
 * Abstract base class for game scenes.
 * Scenes represent distinct game states (menu, gameplay, pause, etc.)
 * Lifecycle: OnEnter() → [Update/Render loop] → OnExit()
 */
class Scene {
public:
    virtual ~Scene() = default;
    
    // Called when scene becomes active (pushed or switched to)
    virtual void OnEnter() {}
    
    // Called when scene is removed (popped or switched away)
    virtual void OnExit() {}
    
    // Called when scene is paused (another scene pushed on top)
    virtual void OnPause() {}
    
    // Called when scene resumes (scene above was popped)
    virtual void OnResume() {}
    
    // Update game logic (called every frame while active)
    virtual void Update(float deltaTime, const Input& input) = 0;
    
    // Render the scene (called every frame while active)
    virtual void Render(Renderer2D& renderer) = 0;
};

} // namespace engine
