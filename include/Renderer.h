#pragma once

#include "SDL2/SDL.h"
#include <iostream>
#include <cstdint>

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

    void debugChunks();

private:
    SDL_Window* window;
    SDL_Surface* surface;
    World* world;
    Camera* camera;
};