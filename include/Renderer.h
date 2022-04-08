#pragma once

#include "SDL2/SDL.h"
#include <iostream>
#include <cstdint>

#include "Camera.h"

#define DEBUG

class Renderer
{
public:
    Renderer();
    Renderer(SDL_Window* _window, Camera* _camera);
    ~Renderer();

    // Make sure you grab the surface every frame
    // (it changes on window resize)
    void getSurface();

    void drawCell(uint32_t color, int x, int y);

private:
    SDL_Window* window;
    SDL_Surface* surface;
    Camera* camera;
};