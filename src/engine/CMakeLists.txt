add_library(engine STATIC
    src/core/Engine.cpp
    src/core/SceneManager.cpp
    src/platform/Window.cpp
    src/platform/Input.cpp
    src/gfx/GLContext.cpp
    src/gfx/GLFunctions.cpp
    src/gfx/Camera2D.cpp
    src/gfx/Shader.cpp
    src/gfx/VertexBuffer.cpp
    src/gfx/IndexBuffer.cpp
    src/gfx/VertexArray.cpp
    src/gfx/Texture2D.cpp
    src/gfx/TextureCache.cpp
    src/gfx/Renderer2D.cpp
    src/gfx/SpriteSheet.cpp
    src/gfx/Tilemap.cpp
    src/utils/JsonParser.cpp
    src/stb_image.cpp
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
)

# SDL3::SDL3 is a target exposed by the SDL submodule
# OpenGL (this works on macOS, Linux, Windows)
find_package(OpenGL REQUIRED)

target_link_libraries(engine PUBLIC
    SDL3::SDL3
    OpenGL::GL
)

