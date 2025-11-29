#include "engine/gfx/Camera2D.h"

namespace engine {

Camera2D::Camera2D() : position(0.0f, 0.0f), zoom(1.0f) {
}

void Camera2D::SetPosition(const Vec2& pos) {
    position = pos;
}

void Camera2D::Move(const Vec2& delta) {
    position = position + delta;
}

void Camera2D::SetViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
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
    
    view.m[12] = -position.x;
    view.m[13] = -position.y;
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

