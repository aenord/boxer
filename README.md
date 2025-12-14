# Boxer

A fairly minimal 2D game-engine-in-progress built with SDL3 and OpenGL 3.3.

## Dependencies

- CMake 3.16+
- C++20 compiler
- OpenGL 3.3

SDL3 and stb_image are included as git submodules.

## Usage

To build:

```bash
cmake -S . -B build
cmake --build build
```

To run whatever my current test/demo is at the moment:

```bash
./build/src/runtime/runtime
```

Make sure to run from the project root so asset paths resolve correctly. 

