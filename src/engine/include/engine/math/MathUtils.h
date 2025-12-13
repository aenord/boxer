#pragma once

#include <cmath>

namespace engine {

// Constants
constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = PI * 2.0f;
constexpr float HALF_PI = PI * 0.5f;

// Angle conversion functions
inline float ToRadians(float degrees) {
    return degrees * (PI / 180.0f);
}

inline float ToDegrees(float radians) {
    return radians * (180.0f / PI);
}

// Trigonometric functions (wrappers for clarity and maybe potential future optimization)
inline float Sin(float radians) { return std::sin(radians); }
inline float Cos(float radians) { return std::cos(radians); }
inline float Tan(float radians) { return std::tan(radians); }
inline float Asin(float value) { return std::asin(value); }
inline float Acos(float value) { return std::acos(value); }
inline float Atan2(float y, float x) { return std::atan2(y, x); }

// Utility
inline float Abs(float value) { return std::abs(value); }
inline float Sqrt(float value) { return std::sqrt(value); }
inline float Min(float a, float b) { return a < b ? a : b; }
inline float Max(float a, float b) { return a > b ? a : b; }
inline float Clamp(float value, float min, float max) {
    return value < min ? min : (value > max ? max : value);
}

// Linear interpolation
inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

} // namespace engine

