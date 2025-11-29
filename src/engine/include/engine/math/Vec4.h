#pragma once

namespace engine {

/**
 * 4D vector for colors (RGBA) and homogeneous coordinates
 */
struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;
    
    Vec4() = default;
    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    
    // Color convenience aliases
    float& r() { return x; }
    float& g() { return y; }
    float& b() { return z; }
    float& a() { return w; }
    const float& r() const { return x; }
    const float& g() const { return y; }
    const float& b() const { return z; }
    const float& a() const { return w; }
    
    Vec4 operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vec4 operator-(const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vec4 operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
};

} // namespace engine

