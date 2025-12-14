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
    
    // Get combined view-projection matrix
    Mat4 GetViewProjectionMatrix() const;
    
    // Update camera (call once per frame for smooth movement)
    void Update(float deltaTime);
    
    // Position manipulation
    void SetPosition(const Vec2& pos);  // Set position immediately (also sets target)
    void SetTarget(const Vec2& target); // Set target for smooth follow
    void Move(const Vec2& delta);       // Move position and target
    void MoveTarget(const Vec2& delta); // Move only target
    Vec2 GetPosition() const { return m_position; }
    Vec2 GetTarget() const { return m_target; }
    
    // Zoom (1.0 = normal, >1.0 = zoomed in, <1.0 = zoomed out)
    void SetZoom(float zoom) { m_zoom = zoom; }
    float GetZoom() const { return m_zoom; }
    
    // Smooth follow settings
    void SetSmoothEnabled(bool enabled) { m_smoothEnabled = enabled; }
    void SetSmoothSpeed(float speed) { m_smoothSpeed = speed; }
    bool IsSmoothEnabled() const { return m_smoothEnabled; }
    float GetSmoothSpeed() const { return m_smoothSpeed; }
    
    // Pixel snapping (for crisp pixel art rendering)
    void SetPixelSnap(bool enabled) { m_pixelSnap = enabled; }
    bool IsPixelSnapEnabled() const { return m_pixelSnap; }
    
    // Viewport dimensions (for projection calculation)
    void SetViewportSize(float width, float height);
    float GetViewportWidth() const { return m_viewportWidth; }
    float GetViewportHeight() const { return m_viewportHeight; }
    
    // World/screen coordinate conversion
    Vec2 ScreenToWorld(const Vec2& screenPos) const;
    Vec2 WorldToScreen(const Vec2& worldPos) const;
    
private:
    Vec2 m_position = Vec2(0.0f, 0.0f);  // Current camera position
    Vec2 m_target = Vec2(0.0f, 0.0f);    // Target for smooth follow
    float m_zoom = 1.0f;
    float m_smoothSpeed = 5.0f;
    bool m_smoothEnabled = false;
    bool m_pixelSnap = false;
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    
    // Generate orthographic projection matrix
    Mat4 GetProjectionMatrix() const;
    
    // Generate view matrix (translation based on position)
    Mat4 GetViewMatrix() const;
    
    // Get position with pixel snapping applied if enabled
    Vec2 GetRenderPosition() const;
};

} // namespace engine

