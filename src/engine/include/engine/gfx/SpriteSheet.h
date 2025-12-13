#pragma once

#include "engine/math/Vec4.h"
#include "engine/gfx/Texture2D.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace engine {

/**
 * Represents a single sprite within a sprite sheet.
 * Contains the UV rectangle for sub-texture rendering.
 */
struct Sprite {
    Vec4 uvRect;       // (minU, minV, maxU, maxV) - normalized texture coordinates
    int pixelWidth;    // Original pixel width (for aspect ratio)
    int pixelHeight;   // Original pixel height
    
    Sprite() : uvRect(0, 0, 1, 1), pixelWidth(0), pixelHeight(0) {}
    Sprite(const Vec4& uv, int w, int h) : uvRect(uv), pixelWidth(w), pixelHeight(h) {}
    
    float GetAspectRatio() const {
        return pixelHeight > 0 ? static_cast<float>(pixelWidth) / pixelHeight : 1.0f;
    }
};

/**
 * A sprite sheet combines a texture atlas with named sprite regions.
 * Load from a JSON file that defines sprite names and their UV coordinates.
 * 
 * JSON format:
 * {
 *   "texture": "path/to/atlas.png",
 *   "sprites": {
 *     "player_idle": { "x": 0, "y": 0, "w": 32, "h": 32 },
 *     "player_run1": { "x": 32, "y": 0, "w": 32, "h": 32 },
 *     ...
 *   }
 * }
 */
class SpriteSheet {
public:
    SpriteSheet() = default;
    ~SpriteSheet() = default;
    
    // Non-copyable, movable
    SpriteSheet(const SpriteSheet&) = delete;
    SpriteSheet& operator=(const SpriteSheet&) = delete;
    SpriteSheet(SpriteSheet&&) = default;
    SpriteSheet& operator=(SpriteSheet&&) = default;
    
    /**
     * Load sprite sheet from JSON file.
     * The JSON should contain texture path and sprite definitions.
     * Returns true on success.
     */
    bool LoadFromFile(const std::string& jsonPath);
    
    /**
     * Create sprite sheet from existing texture with manual sprite definitions.
     * Useful for programmatic sprite sheet creation.
     */
    void SetTexture(std::shared_ptr<Texture2D> texture);
    
    /**
     * Add a sprite region manually (pixel coordinates).
     * Coordinates are converted to normalized UVs internally.
     */
    void AddSprite(const std::string& name, int x, int y, int width, int height);
    
    /**
     * Get a sprite by name.
     * Returns nullptr if sprite not found.
     */
    const Sprite* GetSprite(const std::string& name) const;
    
    /**
     * Check if a sprite exists.
     */
    bool HasSprite(const std::string& name) const;
    
    /**
     * Get the underlying texture.
     */
    const Texture2D* GetTexture() const { return m_texture.get(); }
    Texture2D* GetTexture() { return m_texture.get(); }
    
    /**
     * Check if sprite sheet is valid (has texture).
     */
    bool IsValid() const { return m_texture && m_texture->IsValid(); }
    
    /**
     * Get number of sprites defined.
     */
    size_t GetSpriteCount() const { return m_sprites.size(); }

private:
    std::shared_ptr<Texture2D> m_texture;
    std::unordered_map<std::string, Sprite> m_sprites;
};

} // namespace engine

