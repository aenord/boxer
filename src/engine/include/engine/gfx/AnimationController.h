#pragma once

#include "engine/gfx/Animation.h"
#include "engine/gfx/SpriteSheet.h"
#include <string>
#include <unordered_map>

namespace engine {

// Controls animation playback from a sprite sheet, managing frame timing and transitions
class AnimationController {
public:
    AnimationController() = default;
    ~AnimationController() = default;

    // Non-copyable, movable
    AnimationController(const AnimationController&) = delete;
    AnimationController& operator=(const AnimationController&) = delete;
    AnimationController(AnimationController&&) = default;
    AnimationController& operator=(AnimationController&&) = default;

    // Sets the sprite sheet to use for all animations
    void SetSpriteSheet(const SpriteSheet* sheet) { m_spriteSheet = sheet; }

    // Adds an animation (copies the animation data)
    void AddAnimation(const Animation& animation);

    // Loads animations from a JSON file (see format below)
    bool LoadAnimations(const std::string& jsonPath);

    // Plays the named animation from the beginning
    void Play(const std::string& name);

    // Plays if not already playing this animation (avoids restart)
    void PlayIfNot(const std::string& name);

    // Stops playback, resets to first frame
    void Stop();

    // Pauses playback at current frame
    void Pause();

    // Resumes from paused state
    void Resume();

    // Advances the animation timer
    void Update(float deltaTime);

    // Returns the current sprite to render (or nullptr if none)
    const Sprite* GetCurrentSprite() const;

    // Returns the name of the current sprite (for debugging)
    const std::string& GetCurrentSpriteName() const;

    // Playback state queries
    bool IsPlaying() const { return m_playing && !m_paused; }
    bool IsPaused() const { return m_paused; }
    bool IsFinished() const { return m_finished; }

    // Returns current animation name (empty if none)
    const std::string& GetCurrentAnimationName() const { return m_currentAnimation; }

    // Returns current frame index
    size_t GetCurrentFrameIndex() const { return m_currentFrame; }

    // Playback speed multiplier (1.0 = normal, 2.0 = double speed)
    void SetSpeed(float speed) { m_speed = speed; }
    float GetSpeed() const { return m_speed; }

private:
    const SpriteSheet* m_spriteSheet = nullptr;
    std::unordered_map<std::string, Animation> m_animations;

    std::string m_currentAnimation;
    size_t m_currentFrame = 0;
    float m_frameTimer = 0.0f;
    float m_speed = 1.0f;

    bool m_playing = false;
    bool m_paused = false;
    bool m_finished = false;

    const Animation* GetCurrentAnimation() const;
    void AdvanceFrame();
};

/*
 * JSON Format for LoadAnimations():
 * {
 *   "animations": {
 *     "idle": {
 *       "loop": true,
 *       "frames": [
 *         { "sprite": "player_idle_0", "duration": 0.2 },
 *         { "sprite": "player_idle_1", "duration": 0.2 }
 *       ]
 *     },
 *     "walk": {
 *       "loop": true,
 *       "frames": [
 *         { "sprite": "player_walk_0", "duration": 0.1 },
 *         { "sprite": "player_walk_1", "duration": 0.1 },
 *         { "sprite": "player_walk_2", "duration": 0.1 },
 *         { "sprite": "player_walk_3", "duration": 0.1 }
 *       ]
 *     },
 *     "jump": {
 *       "loop": false,
 *       "frames": [
 *         { "sprite": "player_jump", "duration": 0.5 }
 *       ]
 *     }
 *   }
 * }
 */

} // namespace engine

