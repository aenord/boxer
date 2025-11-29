#pragma once

namespace engine {

/**
 * 4x4 matrix for view-projection transformations
 * Stored in column-major order (OpenGL style)
 * 
 * Matrix layout (column-major):
 *   m[0]  m[4]  m[8]   m[12]   <- column 0
 *   m[1]  m[5]  m[9]   m[13]   <- column 1
 *   m[2]  m[6]  m[10]  m[14]   <- column 2
 *   m[3]  m[7]  m[11]  m[15]   <- column 3
 */
struct Mat4 {
    float m[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // Get pointer to matrix data (for OpenGL uniform upload)
    const float* Data() const { return m; }
    float* Data() { return m; }
    
    // Create identity matrix
    static Mat4 Identity() {
        Mat4 result;
        // Already initialized to identity
        return result;
    }
};

} // namespace engine

