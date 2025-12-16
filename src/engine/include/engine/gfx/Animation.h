#pragma once

#include <string>
#include <vector>

namespace engine {

// A single frame in an animation sequence
struct AnimationFrame {
    std::string spriteName;  // Name of sprite in the sprite sheet
    float duration;          // How long this frame displays (seconds)

    AnimationFrame() : spriteName(), duration(0.1f) {}
    AnimationFrame(const std::string& name, float dur) : spriteName(name), duration(dur) {}
};

// A named sequence of frames with playback settings
struct Animation {
    std::string name;                    // Animation identifier (e.g., "walk", "idle")
    std::vector<AnimationFrame> frames;  // Ordered frame sequence
    bool loop = true;                    // Whether to loop when finished

    Animation() = default;
    Animation(const std::string& n, bool shouldLoop = true) : name(n), loop(shouldLoop) {}

    // Add frame with specified duration
    void AddFrame(const std::string& spriteName, float duration) {
        frames.emplace_back(spriteName, duration);
    }

    // Add multiple frames with uniform duration
    void AddFrames(const std::vector<std::string>& spriteNames, float duration) {
        for (const auto& name : spriteNames) {
            frames.emplace_back(name, duration);
        }
    }

    // Total duration of one animation cycle
    float GetTotalDuration() const {
        float total = 0.0f;
        for (const auto& frame : frames) {
            total += frame.duration;
        }
        return total;
    }

    bool IsEmpty() const { return frames.empty(); }
};

} // namespace engine

