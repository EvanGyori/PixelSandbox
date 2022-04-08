#include "Renderer.h"

Renderer::Renderer()
: window(nullptr), surface(nullptr), camera(nullptr)
{
}

Renderer::Renderer(SDL_Window* _window, Camera* _camera)
: window(_window), surface(nullptr), camera(_camera)
{
    getSurface();
}

Renderer::~Renderer()
{
}

void Renderer::getSurface()
{
    surface = SDL_GetWindowSurface(window);
}

void Renderer::drawCell(uint32_t color, int x, int y)
{
    // w, h - width and height of cell in pixels
    int w = camera->getScale();
    int h = w;

    // sw, sh - screen width and height in pixels
    int sw, sh;
    SDL_GetWindowSize(window, &sw, &sh);

    camera->convertToScreen(x, y, sw, sh);

    #if defined(DEBUG)
        // Check if the cell is within drawing bounds
        if (!(x + w - 1 >= 0 && y + h - 1 >= 0 && x < sw && y < sh)) {
            std::cout << "Attempting to draw rectangles out of bounds" << std::endl;
            return;
        }
    #endif

    // No need for checking if rect is within bounds
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_FillRect(surface, &rect, color);

    /* draw each individual pixel method (Dont use)
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            uint32_t* pixels = (uint32_t*)surface->pixels;
            uint32_t* pixel = pixels + j * surface->pitch/4 + i;
            *pixel = color;
        }
    }
    */
}