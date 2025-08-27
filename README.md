# Minimal C OpenGL Engine

A small rendering engine in C using OpenGL, supporting cameras, materials, and a geometry pass.

## Features

- WIP

## Requirements

- C compiler supporting C11
- [GLFW 3.3+](https://www.glfw.org/)
- [cglm](https://github.com/recp/cglm)
- CMake 3.10+

## Build & Run

Using the provided Makefile:

```bash
# Build the project
make build

# Run the program
make run

# Clean build artifacts
make clean
```

Or manually with CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
./built/game
```

## Project structure
```bash
src/             # Source files
include/         # Header files
assets/          # Misc asset files
```
