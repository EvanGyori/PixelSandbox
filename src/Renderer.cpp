#include "Renderer.h"

Renderer::Renderer()
: window(nullptr), surface(nullptr), world(nullptr), camera(nullptr)
{
}

Renderer::Renderer(SDL_Window* _window, World* _world, Camera* _camera)
: window(_window), surface(nullptr), world(_world), camera(_camera)
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

void Renderer::drawPixel(uint32_t color, int x, int y)
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

void Renderer::debugChunks()
{
    // sw, sh - screen width and height in pixels
    int sw, sh;
    SDL_GetWindowSize(window, &sw, &sh);

    uint32_t color = 0x123FFF;

    std::vector<Chunk>* chunks = world->getChunks();
    for (Chunk& chunk : (*chunks))
    {
        // Chunk coordinates
        int cx = chunk.getX() * Chunk::size;
        int cy = chunk.getY() * Chunk::size;
        camera->convertToScreen(cx, cy, sw, sh);

        SDL_Rect rects[4];

        // Draw top horizontal
        rects[0].x = cx;
        rects[0].y = cy;
        rects[0].w = camera->getScale() * Chunk::size;
        rects[0].h = 1;

        // Draw bottom horizontal
        rects[1].x = cx;
        rects[1].y = cy + camera->getScale() * Chunk::size;
        rects[1].w = rects[0].w;
        rects[1].h = rects[0].h;

        // Draw left vertical
        rects[2].x = cx;
        rects[2].y = cy;
        rects[2].w = 1;
        rects[2].h = camera->getScale() * Chunk::size;

        // Draw right vertical
        rects[3].x = cx + camera->getScale() * Chunk::size;
        rects[3].y = cy;
        rects[3].w = rects[2].w;
        rects[3].h = rects[2].h + 1;

        SDL_FillRects(surface, rects, 4, color);
    }
}