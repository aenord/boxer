cmake_minimum_required(VERSION 3.20)

project(boxer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES_DISABLED_BY_DEFAULT ON CACHE BOOL "" FORCE)
set(SDL_INSTALL_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SDL)

add_subdirectory(src/engine)
add_subdirectory(src/runtime)
