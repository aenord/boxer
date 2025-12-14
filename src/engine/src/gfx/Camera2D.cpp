#include "engine/gfx/Camera2D.h"
#include <cmath>

namespace engine {

Camera2D::Camera2D() : position(0.0f, 0.0f), zoom(1.0f), m_target(0.0f, 0.0f) {
}

void Camera2D::Update(float deltaTime) {
    if (!smoothEnabled || smoothSpeed <= 0.0f) {
        // No smoothing - snap to target immediately
        position = m_target;
        return;
    }
    
    // Lerp toward target position
    // Using exponential smoothing: lerp factor based on deltaTime
    float t = 1.0f - std::exp(-smoothSpeed * deltaTime);
    position = position.Lerp(m_target, t);
}

void Camera2D::SetPosition(const Vec2& pos) {
    position = pos;
    m_target = pos;  // Also update target to prevent snapping
}

void Camera2D::SetTarget(const Vec2& target) {
    m_target = target;
    if (!smoothEnabled) {
        position = target;  // Snap immediately if smoothing disabled
    }
}

void Camera2D::Move(const Vec2& delta) {
    position = position + delta;
    m_target = m_target + delta;
}

void Camera2D::MoveTarget(const Vec2& delta) {
    m_target = m_target + delta;
    if (!smoothEnabled) {
        position = m_target;
    }
}

void Camera2D::SetViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

// Get position with pixel snapping applied if enabled
Vec2 Camera2D::GetRenderPosition() const {
    if (pixelSnap) {
        return Vec2(std::floor(position.x), std::floor(position.y));
    }
    return position;
}

Vec2 Camera2D::ScreenToWorld(const Vec2& screenPos) const {
    // Convert screen coordinates (0,0 = top-left) to world coordinates
    // Screen center = camera position
    Vec2 renderPos = GetRenderPosition();
    float halfWidth = (m_viewportWidth / 2.0f) / zoom;
    float halfHeight = (m_viewportHeight / 2.0f) / zoom;
    
    // Screen to NDC: (0,0) -> (-1,1), (width,height) -> (1,-1)
    float ndcX = (screenPos.x / m_viewportWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (screenPos.y / m_viewportHeight) * 2.0f;  // Flip Y
    
    // NDC to world
    return Vec2(
        renderPos.x + ndcX * halfWidth,
        renderPos.y + ndcY * halfHeight
    );
}

Vec2 Camera2D::WorldToScreen(const Vec2& worldPos) const {
    // Convert world coordinates to screen coordinates
    Vec2 renderPos = GetRenderPosition();
    float halfWidth = (m_viewportWidth / 2.0f) / zoom;
    float halfHeight = (m_viewportHeight / 2.0f) / zoom;
    
    // World to NDC
    float ndcX = (worldPos.x - renderPos.x) / halfWidth;
    float ndcY = (worldPos.y - renderPos.y) / halfHeight;
    
    // NDC to screen (flip Y)
    return Vec2(
        (ndcX + 1.0f) * 0.5f * m_viewportWidth,
        (1.0f - ndcY) * 0.5f * m_viewportHeight
    );
}

Mat4 Camera2D::GetProjectionMatrix() const {
    // Create orthographic projection matrix
    // Maps world coordinates to normalized device coordinates [-1, 1]
    Mat4 proj;
    
    // Calculate half-width and half-height based on zoom
    float halfWidth = (m_viewportWidth / 2.0f) / zoom;
    float halfHeight = (m_viewportHeight / 2.0f) / zoom;
    
    // Orthographic projection: maps [left, right] x [bottom, top] to [-1, 1] x [-1, 1]
    float left = -halfWidth;
    float right = halfWidth;
    float bottom = -halfHeight;
    float top = halfHeight;
    float near = -1.0f;
    float far = 1.0f;
    
    // Column-major order (OpenGL style)
    proj.m[0] = 2.0f / (right - left);
    proj.m[1] = 0.0f;
    proj.m[2] = 0.0f;
    proj.m[3] = 0.0f;
    
    proj.m[4] = 0.0f;
    proj.m[5] = 2.0f / (top - bottom);
    proj.m[6] = 0.0f;
    proj.m[7] = 0.0f;
    
    proj.m[8] = 0.0f;
    proj.m[9] = 0.0f;
    proj.m[10] = -2.0f / (far - near);
    proj.m[11] = 0.0f;
    
    proj.m[12] = -(right + left) / (right - left);
    proj.m[13] = -(top + bottom) / (top - bottom);
    proj.m[14] = -(far + near) / (far - near);
    proj.m[15] = 1.0f;
    
    return proj;
}

Mat4 Camera2D::GetViewMatrix() const {
    // Create view matrix (translation by negative camera position)
    // This moves the world so the camera appears at the origin
    // Use render position for pixel snapping
    Vec2 renderPos = GetRenderPosition();
    Mat4 view;
    
    // Translation matrix (column-major)
    view.m[0] = 1.0f;
    view.m[1] = 0.0f;
    view.m[2] = 0.0f;
    view.m[3] = 0.0f;
    
    view.m[4] = 0.0f;
    view.m[5] = 1.0f;
    view.m[6] = 0.0f;
    view.m[7] = 0.0f;
    
    view.m[8] = 0.0f;
    view.m[9] = 0.0f;
    view.m[10] = 1.0f;
    view.m[11] = 0.0f;
    
    view.m[12] = -renderPos.x;
    view.m[13] = -renderPos.y;
    view.m[14] = 0.0f;
    view.m[15] = 1.0f;
    
    return view;
}

Mat4 Camera2D::GetViewProjectionMatrix() const {
    // Combine view and projection: VP = Projection * View
    Mat4 proj = GetProjectionMatrix();
    Mat4 view = GetViewMatrix();
    Mat4 result;
    
    // Matrix multiplication: result = proj * view
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += proj.m[i + k * 4] * view.m[k + j * 4];
            }
            result.m[i + j * 4] = sum;
        }
    }
    
    return result;
}

} // namespace engine
