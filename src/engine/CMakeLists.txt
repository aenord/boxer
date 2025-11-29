add_library(engine STATIC
    src/core/Engine.cpp
    src/platform/Window.cpp
    src/platform/Input.cpp
    src/gfx/GLContext.cpp
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# SDL3::SDL3 is a target exposed by the SDL submodule
# OpenGL (this works on macOS, Linux, Windows)
find_package(OpenGL REQUIRED)

target_link_libraries(engine PUBLIC
    SDL3::SDL3
    OpenGL::GL
)

