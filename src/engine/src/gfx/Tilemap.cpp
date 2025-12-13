#include "engine/gfx/Tilemap.h"
#include "engine/gfx/SpriteSheet.h"
#include "engine/gfx/Renderer2D.h"
#include "engine/math/Vec4.h"

namespace engine {

Tilemap::Tilemap(int width, int height, float tileSize)
    : m_width(width)
    , m_height(height)
    , m_tileSize(tileSize)
    , m_tiles(width * height, EMPTY_TILE)
{
}

void Tilemap::SetSpriteSheet(const SpriteSheet* spriteSheet) {
    m_spriteSheet = spriteSheet;
}

void Tilemap::SetTileSprite(int32_t tileIndex, const std::string& spriteName) {
    if (tileIndex < 0) return;
    
    // Grow vector if needed
    if (static_cast<size_t>(tileIndex) >= m_tileSprites.size()) {
        m_tileSprites.resize(tileIndex + 1);
    }
    
    m_tileSprites[tileIndex] = spriteName;
}

void Tilemap::SetTile(int x, int y, int32_t tileIndex) {
    if (InBounds(x, y)) {
        m_tiles[Index(x, y)] = tileIndex;
    }
}

int32_t Tilemap::GetTile(int x, int y) const {
    if (InBounds(x, y)) {
        return m_tiles[Index(x, y)];
    }
    return EMPTY_TILE;
}

void Tilemap::Fill(int32_t tileIndex) {
    std::fill(m_tiles.begin(), m_tiles.end(), tileIndex);
}

void Tilemap::Clear() {
    Fill(EMPTY_TILE);
}

void Tilemap::Draw(Renderer2D& renderer, const Vec2& offset) const {
    if (!m_spriteSheet || !m_spriteSheet->IsValid()) {
        return;
    }
    
    const Texture2D* texture = m_spriteSheet->GetTexture();
    if (!texture) return;
    
    Vec2 size(m_tileSize, m_tileSize);
    Vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Half tile offset for centering (DrawQuad draws from center)
    float halfTile = m_tileSize * 0.5f;
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int32_t tileIndex = m_tiles[Index(x, y)];
            
            // Skip empty tiles
            if (tileIndex == EMPTY_TILE) continue;
            
            // Skip if no sprite mapped for this tile index
            if (tileIndex < 0 || static_cast<size_t>(tileIndex) >= m_tileSprites.size()) continue;
            
            const std::string& spriteName = m_tileSprites[tileIndex];
            if (spriteName.empty()) continue;
            
            const Sprite* sprite = m_spriteSheet->GetSprite(spriteName);
            if (!sprite) continue;
            
            // Calculate world position (center of tile)
            Vec2 pos(
                offset.x + x * m_tileSize + halfTile,
                offset.y + y * m_tileSize + halfTile
            );
            
            renderer.DrawQuad(pos, size, *texture, sprite->uvRect, white);
        }
    }
}

Vec2 Tilemap::WorldToGrid(const Vec2& worldPos, const Vec2& offset) const {
    return Vec2(
        (worldPos.x - offset.x) / m_tileSize,
        (worldPos.y - offset.y) / m_tileSize
    );
}

Vec2 Tilemap::GridToWorld(int x, int y, const Vec2& offset) const {
    float halfTile = m_tileSize * 0.5f;
    return Vec2(
        offset.x + x * m_tileSize + halfTile,
        offset.y + y * m_tileSize + halfTile
    );
}

} // namespace engine

