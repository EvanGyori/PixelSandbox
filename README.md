<p align="center"> <img src="https://github.com/EvanGyori/PixelSandbox/assets/30540753/1cf6c17d-b9e2-4c27-a466-201349501b3c"> </p>

# Pixel Sandbox
A sandbox where each pixel of sand falls down onto a platform and forms a pyramid. The rest of the sand slides off and falls into an infinite void.

## Overview
Each pixel is stored in a map of chunks to allow for infinite size. By storing in chunks, this allows for multithreading for optimization. However, the multithreading hasn't been implemented. These chunks are created whenever a pixel of sand falls outside of its current chunk.

The program uses SDL for drawing and window creation.

All solutions to the pixel sandbox I found online would always allow pixels to disappear for easier implementation but I used a step by step process to allow every pixel to move each frame without being deleted. The conservation of pixels.

## Controls
Use WASD to pan view
<p>Use mouse scroll wheel to zoom in and out
<p>Space pauses the simulation

## Usage
This program requires [SDL2](https://wiki.libsdl.org/SDL2/Installation) and CMake. Make sure `SDL2Config.cmake` can be found by CMake.

Create a build directory and run cmake from it
```
cmake <project-source-directory>
cmake --build .
```
Creates an executable called `PixelSandbox`. Put the required SDL2 dll file with this executable in order to run it.
