#pragma once

namespace engine {

/**
 * 3x3 matrix for 2D transformations
 * Stored in column-major order (OpenGL style)
 * 
 * Matrix layout (column-major):
 *   m[0]  m[3]  m[6]   <- column 0
 *   m[1]  m[4]  m[7]   <- column 1
 *   m[2]  m[5]  m[8]   <- column 2
 */
struct Mat3 {
    float m[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    
    // Get pointer to matrix data (for OpenGL uniform upload)
    const float* Data() const { return m; }
    float* Data() { return m; }
    
    // Create identity matrix
    static Mat3 Identity() {
        Mat3 result;
        // Already initialized to identity
        return result;
    }
};

} // namespace engine

