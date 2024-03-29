#pragma once

#include "SDL2/SDL.h"
#include <iostream>
#include <cstdint>

#include "Settings.h"
#include "Camera.h"
#include "World.h"

//#define DEBUG

class Renderer
{
public:
    Renderer();
    Renderer(SDL_Window* _window, World* _world, Camera* _camera);
    ~Renderer();

    // Make sure you grab the surface every frame
    // (it changes on window resize)
    void getSurface();

    void drawPixel(uint32_t color, int x, int y);

    void draw();
    void drawChunk(Chunk& chunk);
    void debugChunk(Chunk& chunk, uint32_t color);

private:
    SDL_Window* window;
    SDL_Surface* surface;
    World* world;
    Camera* camera;

    // Screen width and height, updates every frame
    int sw, sh;
};
