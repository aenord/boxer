#pragma once

#include "engine/math/MathUtils.h"

namespace engine {

/**
 * Simple 2D vector for positions, directions, and transformations
 */
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
    
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}
    
    // Arithmetic operators
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
    
    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    
    // Negation
    Vec2 operator-() const { return Vec2(-x, -y); }
    
    // Rotation (returns new rotated vector)
    Vec2 Rotated(float radians) const {
        float c = Cos(radians);
        float s = Sin(radians);
        return Vec2(x * c - y * s, x * s + y * c);
    }
    
    // Rotate around a pivot point
    Vec2 RotatedAround(const Vec2& pivot, float radians) const {
        Vec2 relative = *this - pivot;
        Vec2 rotated = relative.Rotated(radians);
        return rotated + pivot;
    }
};

} // namespace engine

