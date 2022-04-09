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

void Renderer::draw()
{
    SDL_GetWindowSize(window, &sw, &sh);

    ChunkMap* chunks = world->getChunks();

    /*
    // csw and csh - chunk screen width and height,
    // number of chunks that span across the screen
    int csw = sw / (camera->getScale() * Chunk::size);
    int csh = sh / (camera->getScale() * Chunk::size);

    int startX = camera->getX() / Chunk::size - csw/2;
    int startY = camera->getY() / Chunk::size - csh/2;

    int endX = startX + csw;
    int endY = startY + csh;
    */

    
    // The camera conversion function will
    // find the edges of the screen to only
    // iterate through the chunks on the screen
    int startX = 0;
    int startY = 0;
    int endX = sw;
    int endY = sh;

    camera->convertToWorld(startX, startY, sw, sh);
    camera->convertToWorld(endX, endY, sw, sh);

    // Divide by chunk size to get chunk coordinates
    startX /= Chunk::size; startY /= Chunk::size;
    endX /= Chunk::size; endY /= Chunk::size;
    
    for (int x = startX - 1; x < endX + 1; x++) {
        for (int y = startY - 1; y < endY + 1; y++) {
            auto key = std::make_pair(x, y);
            if (chunks->find(key) != chunks->end()) {
                drawChunk((*chunks)[key]);
                debugChunk((*chunks)[key], 0x123fff);
            }
        }
    }
}

void Renderer::drawChunk(Chunk& chunk)
{
    int cx = chunk.x * Chunk::size;
    int cy = chunk.y * Chunk::size;

    for (int x = 0; x < Chunk::size; x++) {
        for (int y = 0; y < Chunk::size; y++) {
            int index = x + y * Chunk::size;
            drawPixel(chunk.cells[index].color, x + cx, y + cy);
        }
    }
}

void Renderer::debugChunk(Chunk& chunk, uint32_t color)
{
    // Chunk coordinates
    int cx = chunk.x * Chunk::size;
    int cy = chunk.y * Chunk::size;
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