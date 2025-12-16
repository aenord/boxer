#pragma once

#include "engine/math/Vec2.h"

namespace engine {

// Axis-Aligned Bounding Box for collision detection
struct AABB {
    Vec2 min;  // Bottom-left corner
    Vec2 max;  // Top-right corner

    AABB() : min(0, 0), max(0, 0) {}
    AABB(const Vec2& minPt, const Vec2& maxPt) : min(minPt), max(maxPt) {}

    // Creates AABB from bottom-left position and size
    static AABB FromPositionSize(const Vec2& position, const Vec2& size) {
        return AABB(position, position + size);
    }

    // Creates AABB from center point and half-extents
    static AABB FromCenter(const Vec2& center, const Vec2& halfSize) {
        return AABB(center - halfSize, center + halfSize);
    }

    Vec2 GetCenter() const { return (min + max) * 0.5f; }
    Vec2 GetSize() const { return max - min; }
    Vec2 GetHalfSize() const { return GetSize() * 0.5f; }

    // Returns true if point is inside the box
    bool Contains(const Vec2& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y;
    }

    // Returns true if this box overlaps another
    bool Intersects(const AABB& other) const {
        return min.x <= other.max.x && max.x >= other.min.x &&
               min.y <= other.max.y && max.y >= other.min.y;
    }

    // Expands the box to include a point
    void Encapsulate(const Vec2& point) {
        if (point.x < min.x) min.x = point.x;
        if (point.y < min.y) min.y = point.y;
        if (point.x > max.x) max.x = point.x;
        if (point.y > max.y) max.y = point.y;
    }

    // Expands the box to include another box
    void Encapsulate(const AABB& other) {
        Encapsulate(other.min);
        Encapsulate(other.max);
    }

    // Returns a box expanded by the given amount on all sides
    AABB Expanded(float amount) const {
        return AABB(min - Vec2(amount, amount), max + Vec2(amount, amount));
    }

    // Moves the box by offset
    AABB Translated(const Vec2& offset) const {
        return AABB(min + offset, max + offset);
    }
};

// Collision query functions
namespace Collision {

    // Point tests
    inline bool PointInAABB(const Vec2& point, const AABB& box) {
        return box.Contains(point);
    }

    // Shape vs shape tests
    inline bool AABBvsAABB(const AABB& a, const AABB& b) {
        return a.Intersects(b);
    }

} // namespace Collision

} // namespace engine

