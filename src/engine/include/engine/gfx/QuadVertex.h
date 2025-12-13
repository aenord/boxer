#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Vec4.h"

namespace engine {

/**
 * Per-vertex data for batched quad rendering
 * Layout matches the vertex attribute configuration in Renderer2D
 */
struct QuadVertex {
    Vec2 position;   // World-space position (after transform)
    Vec2 texCoord;   // UV coordinates
    Vec4 color;      // Vertex color (includes tint)
    float texIndex;  // Texture slot index (0-15) for multi-texture batching
};

// A quad consists of 4 vertices
constexpr int VERTICES_PER_QUAD = 4;

// A quad is drawn with 6 indices (2 triangles)
constexpr int INDICES_PER_QUAD = 6;

} // namespace engine

