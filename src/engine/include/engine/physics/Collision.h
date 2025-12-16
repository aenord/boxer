#pragma once

#include "engine/math/Vec2.h"

namespace engine {

// Axis-Aligned Bounding Box 
struct AABB {
    Vec2 min;  // Bottom-left corner
    Vec2 max;  // Top-right corner

    AABB() : min(0, 0), max(0, 0) {}
    AABB(const Vec2& minPt, const Vec2& maxPt) : min(minPt), max(maxPt) {}

    // Creates AABB from bottom-left position and size
    static AABB FromPositionSize(const Vec2& position, const Vec2& size) {
        return AABB(position, position + size);
    }

    // Creates AABB from center and radius
    static AABB FromCenter(const Vec2& center, const Vec2& halfSize) {
        return AABB(center - halfSize, center + halfSize);
    }

    Vec2 GetCenter() const { return (min + max) * 0.5f; }
    Vec2 GetSize() const { return max - min; }
    Vec2 GetHalfSize() const { return GetSize() * 0.5f; }

    // Returns true if point is inside the AABB
    bool Contains(const Vec2& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y;
    }

    // Returns true if boxes overlap
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

    // Returns a box expanded on all sides
    AABB Expanded(float amount) const {
        return AABB(min - Vec2(amount, amount), max + Vec2(amount, amount));
    }

    // Moves the box
    AABB Translated(const Vec2& offset) const {
        return AABB(min + offset, max + offset);
    }
};

// Detailed collision information
struct CollisionInfo {
    bool hit;           // Whether collision occurred
    Vec2 normal;        // Direction to push A out of B
    float penetration;  // How far A is overlapping B (positive when overlapping)

    CollisionInfo() : hit(false), normal(0, 0), penetration(0) {}
};

// Collision query and resolution functions
namespace Collision {

    // Point tests
    inline bool PointInAABB(const Vec2& point, const AABB& box) {
        return box.Contains(point);
    }

    // Quick overlap test (no details)
    inline bool AABBvsAABB(const AABB& a, const AABB& b) {
        return a.Intersects(b);
    }

    // Collision info between two AABBs
    // Returns penetration depth and push direction (normal points from B to A)
    inline CollisionInfo GetCollisionInfo(const AABB& a, const AABB& b) {
        CollisionInfo info;

        // Check for overlap
        if (!a.Intersects(b)) {
            return info;  // No collision, return default info
        }

        info.hit = true;

        float overlapLeft = a.max.x - b.min.x;    
        float overlapRight = b.max.x - a.min.x;   
        float overlapBottom = a.max.y - b.min.y;  
        float overlapTop = b.max.y - a.min.y;    

        // Find minimum overlap and corresponding normal
        float minOverlap = overlapLeft;
        info.normal = Vec2(-1, 0);  // Push A left

        if (overlapRight < minOverlap) {
            minOverlap = overlapRight;
            info.normal = Vec2(1, 0);  // Push A right
        }
        if (overlapBottom < minOverlap) {
            minOverlap = overlapBottom;
            info.normal = Vec2(0, -1);  // Push A down
        }
        if (overlapTop < minOverlap) {
            minOverlap = overlapTop;
            info.normal = Vec2(0, 1);  // Push A up
        }

        info.penetration = minOverlap;
        return info;
    }

    // Returns the displacement vector to separate A from B (zero if no overlap)
    // To move A out of B: position += GetSeparation(myBox, obstacleBox)
    inline Vec2 GetSeparation(const AABB& a, const AABB& b) {
        CollisionInfo info = GetCollisionInfo(a, b);
        if (!info.hit) {
            return Vec2(0, 0);
        }
        return info.normal * info.penetration;
    }

} // namespace Collision

} // namespace engine

