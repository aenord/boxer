#pragma once

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_log.h>

namespace engine {

/**
 * Check for OpenGL errors (debug builds only)
 * Call after GL operations to catch errors early
 * 
 * Usage:
 *   glDrawElements(...);
 *   GL_CHECK_ERROR();
 */
#ifdef NDEBUG
    // Release build: no-op
    #define GL_CHECK_ERROR() ((void)0)
#else
    // Debug build: check and log errors
    #define GL_CHECK_ERROR() ::engine::CheckGLError(__FILE__, __LINE__)
#endif

/**
 * Check for GL errors and log them
 * Returns true if an error occurred
 */
inline bool CheckGLError(const char* file, int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        const char* errorStr = "UNKNOWN";
        switch (error) {
            case GL_INVALID_ENUM:      errorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     errorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:     errorStr = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: 
                                       errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        SDL_Log("OpenGL error %s (0x%x) at %s:%d", errorStr, error, file, line);
        return true;
    }
    return false;
}

/**
 * Assert that OpenGL context is valid
 * Call before operations that require GL context
 */
inline bool IsGLContextValid() {
    // Try a simple GL call - if it fails, context isn't ready
    // glGetError() returns GL_INVALID_OPERATION if no context
    glGetError();  // Clear any pending errors
    
    // Check if we can query the GL version (requires valid context)
    const GLubyte* version = glGetString(GL_VERSION);
    return version != nullptr;
}

} // namespace engine

