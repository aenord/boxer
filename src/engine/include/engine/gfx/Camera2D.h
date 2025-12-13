#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Mat4.h"

namespace engine {

/**
 * 2D camera with orthographic projection
 * Manages view and projection matrices for 2D rendering
 * Supports smooth following and pixel snapping
 */
class Camera2D {
public:
    Camera2D();
    
    // Camera properties (read-only for smooth mode, use SetPosition/SetTarget)
    Vec2 position;  // Current camera position in world space
    float zoom;     // Zoom level (1.0 = normal, >1.0 = zoomed in, <1.0 = zoomed out)
    
    // Smooth follow settings
    float smoothSpeed = 5.0f;   // Lerp speed (higher = faster, 0 = instant)
    bool smoothEnabled = false; // Enable smooth following
    bool pixelSnap = false;     // Snap position to integer pixels for crisp pixel art
    
    // Get combined view-projection matrix
    Mat4 GetViewProjectionMatrix() const;
    
    // Update camera (call once per frame for smooth movement)
    void Update(float deltaTime);
    
    // Camera manipulation
    void SetPosition(const Vec2& pos);  // Set position immediately
    void SetTarget(const Vec2& target); // Set target for smooth follow
    void Move(const Vec2& delta);       // Move relative to current position
    void MoveTarget(const Vec2& delta); // Move target relative to current target
    
    // Get current target position
    Vec2 GetTarget() const { return m_target; }
    
    // Set viewport dimensions (for projection calculation)
    void SetViewportSize(float width, float height);
    float GetViewportWidth() const { return m_viewportWidth; }
    float GetViewportHeight() const { return m_viewportHeight; }
    
    // World/screen coordinate conversion
    Vec2 ScreenToWorld(const Vec2& screenPos) const;
    Vec2 WorldToScreen(const Vec2& worldPos) const;
    
private:
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    Vec2 m_target;  // Target position for smooth follow
    
    // Generate orthographic projection matrix
    Mat4 GetProjectionMatrix() const;
    
    // Generate view matrix (translation based on position)
    Mat4 GetViewMatrix() const;
    
    // Get position with pixel snapping applied if enabled
    Vec2 GetRenderPosition() const;
};

} // namespace engine

