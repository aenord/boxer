#pragma once

#include "engine/math/Vec2.h"
#include <vector>
#include <string>
#include <cstdint>

namespace engine {

class SpriteSheet;
class Renderer2D;

/**
 * A 2D grid of tiles that renders efficiently using batch rendering.
 * Each tile is an index into a sprite sheet.
 * 
 * Usage:
 *   Tilemap map(20, 15, 32.0f);  // 20x15 tiles, 32px each
 *   map.SetSpriteSheet(&spriteSheet);
 *   map.SetTileSprite(0, "grass");   // Tile index 0 = "grass" sprite
 *   map.SetTile(5, 3, 0);            // Place grass at (5, 3)
 *   map.Draw(renderer, offset);
 */
class Tilemap {
public:
    static constexpr int32_t EMPTY_TILE = -1;
    
    /**
     * Create a tilemap with given dimensions.
     * @param width Number of tiles horizontally
     * @param height Number of tiles vertically
     * @param tileSize Size of each tile in world units (pixels)
     */
    Tilemap(int width, int height, float tileSize);
    ~Tilemap() = default;
    
    // Non-copyable, movable
    Tilemap(const Tilemap&) = delete;
    Tilemap& operator=(const Tilemap&) = delete;
    Tilemap(Tilemap&&) = default;
    Tilemap& operator=(Tilemap&&) = default;
    
    // Sprite sheet binding
    void SetSpriteSheet(const SpriteSheet* spriteSheet);
    const SpriteSheet* GetSpriteSheet() const { return m_spriteSheet; }
    
    /**
     * Map a tile index to a sprite name in the sprite sheet.
     * @param tileIndex The tile type (0, 1, 2, ...)
     * @param spriteName Name of the sprite in the sprite sheet
     */
    void SetTileSprite(int32_t tileIndex, const std::string& spriteName);
    
    /**
     * Set the tile at a grid position.
     * @param x Grid X coordinate (0 to width-1)
     * @param y Grid Y coordinate (0 to height-1)
     * @param tileIndex Tile type index, or EMPTY_TILE for no tile
     */
    void SetTile(int x, int y, int32_t tileIndex);
    
    /**
     * Get the tile at a grid position.
     * Returns EMPTY_TILE if out of bounds or empty.
     */
    int32_t GetTile(int x, int y) const;
    
    /**
     * Fill the entire map with a single tile type.
     */
    void Fill(int32_t tileIndex);
    
    /**
     * Clear the entire map (set all tiles to EMPTY_TILE).
     */
    void Clear();
    
    /**
     * Draw the tilemap using the renderer.
     * @param renderer The 2D renderer
     * @param offset World position offset for the tilemap origin (bottom-left)
     */
    void Draw(Renderer2D& renderer, const Vec2& offset = Vec2(0.0f, 0.0f)) const;
    
    // Dimensions
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    float GetTileSize() const { return m_tileSize; }
    
    // World size
    float GetWorldWidth() const { return m_width * m_tileSize; }
    float GetWorldHeight() const { return m_height * m_tileSize; }
    
    /**
     * Convert world position to grid coordinates.
     * @param worldPos Position in world space
     * @param offset Tilemap offset (same as passed to Draw)
     * @return Grid coordinates (may be out of bounds)
     */
    Vec2 WorldToGrid(const Vec2& worldPos, const Vec2& offset = Vec2(0.0f, 0.0f)) const;
    
    /**
     * Convert grid coordinates to world position (center of tile).
     */
    Vec2 GridToWorld(int x, int y, const Vec2& offset = Vec2(0.0f, 0.0f)) const;

private:
    int m_width;
    int m_height;
    float m_tileSize;
    
    std::vector<int32_t> m_tiles;                          // Grid data (row-major)
    std::vector<std::string> m_tileSprites;                // Tile index → sprite name
    const SpriteSheet* m_spriteSheet = nullptr;            // Non-owning pointer
    
    // Convert 2D coords to 1D index
    int Index(int x, int y) const { return y * m_width + x; }
    bool InBounds(int x, int y) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }
};

} // namespace engine

