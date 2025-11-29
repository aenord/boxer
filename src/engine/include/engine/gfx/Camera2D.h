#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Mat4.h"

namespace engine {

/**
 * 2D camera with orthographic projection
 * Manages view and projection matrices for 2D rendering
 */
class Camera2D {
public:
    Camera2D();
    
    // Camera properties
    Vec2 position;  // Camera position in world space
    float zoom;     // Zoom level (1.0 = normal, >1.0 = zoomed in, <1.0 = zoomed out)
    
    // Get combined view-projection matrix
    Mat4 GetViewProjectionMatrix() const;
    
    // Camera manipulation
    void SetPosition(const Vec2& pos);
    void Move(const Vec2& delta);
    
    // Set viewport dimensions (for projection calculation)
    void SetViewportSize(float width, float height);
    
private:
    float m_viewportWidth = 800.0f;
    float m_viewportHeight = 600.0f;
    
    // Generate orthographic projection matrix
    Mat4 GetProjectionMatrix() const;
    
    // Generate view matrix (translation based on position)
    Mat4 GetViewMatrix() const;
};

} // namespace engine

