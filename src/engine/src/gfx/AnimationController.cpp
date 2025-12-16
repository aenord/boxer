#include "engine/gfx/AnimationController.h"
#include "engine/utils/JsonParser.h"
#include <SDL3/SDL_log.h>

namespace engine {

void AnimationController::AddAnimation(const Animation& animation) {
    if (animation.name.empty()) {
        SDL_Log("AnimationController: Cannot add animation with empty name");
        return;
    }
    m_animations[animation.name] = animation;
}

bool AnimationController::LoadAnimations(const std::string& jsonPath) {
    JsonValue root;
    if (!JsonParser::ParseFile(jsonPath, root)) {
        SDL_Log("AnimationController: Failed to parse '%s'", jsonPath.c_str());
        return false;
    }

    if (!root.IsObject() || !root.HasKey("animations") || !root["animations"].IsObject()) {
        SDL_Log("AnimationController: Invalid format in '%s' (expected 'animations' object)", jsonPath.c_str());
        return false;
    }

    const JsonValue& animations = root["animations"];
    int loadedCount = 0;

    for (const auto& [animName, animValue] : animations) {
        if (!animValue.IsObject()) continue;

        Animation anim(animName);
        
        // Parse loop setting (defaults to true)
        if (animValue.HasKey("loop") && animValue["loop"].IsBool()) {
            anim.loop = animValue["loop"].AsBool();
        }

        // Parse frames
        if (!animValue.HasKey("frames") || !animValue["frames"].IsArray()) {
            SDL_Log("AnimationController: Animation '%s' missing frames array", animName.c_str());
            continue;
        }

        const JsonValue& frames = animValue["frames"];
        for (size_t i = 0; i < frames.Size(); ++i) {
            const JsonValue& frameValue = frames[i];
            if (!frameValue.IsObject()) continue;

            std::string spriteName;
            float duration = 0.1f;

            if (frameValue.HasKey("sprite") && frameValue["sprite"].IsString()) {
                spriteName = frameValue["sprite"].AsString();
            } else {
                SDL_Log("AnimationController: Frame %zu in '%s' missing sprite name", i, animName.c_str());
                continue;
            }

            if (frameValue.HasKey("duration") && frameValue["duration"].IsNumber()) {
                duration = frameValue["duration"].AsFloat();
            }

            anim.AddFrame(spriteName, duration);
        }

        if (!anim.IsEmpty()) {
            m_animations[animName] = anim;
            ++loadedCount;
        }
    }

    SDL_Log("AnimationController: Loaded %d animations from '%s'", loadedCount, jsonPath.c_str());
    return loadedCount > 0;
}

void AnimationController::Play(const std::string& name) {
    auto it = m_animations.find(name);
    if (it == m_animations.end()) {
        SDL_Log("AnimationController: Animation '%s' not found", name.c_str());
        return;
    }

    m_currentAnimation = name;
    m_currentFrame = 0;
    m_frameTimer = 0.0f;
    m_playing = true;
    m_paused = false;
    m_finished = false;
}

void AnimationController::PlayIfNot(const std::string& name) {
    if (m_currentAnimation != name || m_finished) {
        Play(name);
    }
}

void AnimationController::Stop() {
    m_playing = false;
    m_paused = false;
    m_finished = false;
    m_currentFrame = 0;
    m_frameTimer = 0.0f;
}

void AnimationController::Pause() {
    if (m_playing) {
        m_paused = true;
    }
}

void AnimationController::Resume() {
    if (m_playing) {
        m_paused = false;
    }
}

void AnimationController::Update(float deltaTime) {
    if (!m_playing || m_paused || m_finished) {
        return;
    }

    const Animation* anim = GetCurrentAnimation();
    if (!anim || anim->IsEmpty()) {
        return;
    }

    // Advance timer with speed multiplier
    m_frameTimer += deltaTime * m_speed;

    // Check if current frame duration has elapsed
    const AnimationFrame& currentFrame = anim->frames[m_currentFrame];
    while (m_frameTimer >= currentFrame.duration) {
        m_frameTimer -= currentFrame.duration;
        AdvanceFrame();

        // Re-check animation state after advancing (may have finished)
        if (m_finished) {
            break;
        }
    }
}

const Sprite* AnimationController::GetCurrentSprite() const {
    if (!m_spriteSheet) {
        return nullptr;
    }

    const Animation* anim = GetCurrentAnimation();
    if (!anim || anim->IsEmpty()) {
        return nullptr;
    }

    const std::string& spriteName = anim->frames[m_currentFrame].spriteName;
    return m_spriteSheet->GetSprite(spriteName);
}

const std::string& AnimationController::GetCurrentSpriteName() const {
    static const std::string empty;
    
    const Animation* anim = GetCurrentAnimation();
    if (!anim || anim->IsEmpty()) {
        return empty;
    }

    return anim->frames[m_currentFrame].spriteName;
}

const Animation* AnimationController::GetCurrentAnimation() const {
    if (m_currentAnimation.empty()) {
        return nullptr;
    }
    
    auto it = m_animations.find(m_currentAnimation);
    return it != m_animations.end() ? &it->second : nullptr;
}

void AnimationController::AdvanceFrame() {
    const Animation* anim = GetCurrentAnimation();
    if (!anim) return;

    m_currentFrame++;

    if (m_currentFrame >= anim->frames.size()) {
        if (anim->loop) {
            // Loop back to start
            m_currentFrame = 0;
        } else {
            // Stay on last frame, mark finished
            m_currentFrame = anim->frames.size() - 1;
            m_finished = true;
        }
    }
}

} // namespace engine

