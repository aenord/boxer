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
    
    // Length and distance
    float LengthSquared() const { return x * x + y * y; }
    float Length() const { return Sqrt(LengthSquared()); }
    
    float DistanceSquared(const Vec2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }
    
    float Distance(const Vec2& other) const {
        return Sqrt(DistanceSquared(other));
    }
    
    // Normalization
    Vec2 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0.0f, 0.0f);
    }
    
    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }
    
    // Dot product
    float Dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }
    
    // Linear interpolation toward another vector
    Vec2 Lerp(const Vec2& target, float t) const {
        return Vec2(
            x + (target.x - x) * t,
            y + (target.y - y) * t
        );
    }
    
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

