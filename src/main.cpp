/*
Debug Terminal Command:
g++ -o debug -Wall -I ./include/ src/main.cpp src/Cell.cpp src/Renderer.cpp src/Camera.cpp src/InputHandler.cpp src/Chunk.cpp src/World.cpp src/Element.cpp src/CellAutomaton.cpp -lSDL2

*/


#include "SDL2/SDL.h"
#include <iostream>
#include <chrono>
#include <thread>

#include "CellAutomaton.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "World.h"
#include "Element.h"

#define DEBUG


namespace ch = std::chrono;
typedef ch::system_clock::time_point ch_time;

ch_time frameTimer;

// minimum time per frame in seconds
const float refreshRateCap = 0.3f;

// how long mainloop took to complete
float frameTime;

// how long the main thread was put to sleep for
// to meet refreshRateCap
float timeSlept;

// total time since last mainloop call, combined
// timeSlept + frameTime, used for calculations
float deltaTime;

SDL_Window* window;
SDL_Surface* surface;
SDL_Event event;

World world;
Renderer renderer;
Camera mainCamera;
InputHandler inputHandler;

bool running;

void initInputHandler()
{
    inputHandler.addEvent("zoom", [&]() {
        mainCamera.zoom(event.wheel.y);
    });

    inputHandler.addEvent("moveUp", [&]() {
        mainCamera.move(0.0f, -deltaTime);
    });

    inputHandler.addEvent("moveDown", [&]() {
        mainCamera.move(0.0f, deltaTime);
    });

    inputHandler.addEvent("moveLeft", [&]() {
        mainCamera.move(-deltaTime, 0.0f);
    });

    inputHandler.addEvent("moveRight", [&]() {
        mainCamera.move(deltaTime, 0.0f);
    });
}

void initElements()
{
    Element::elements[ELEMENTS::EMPTY] =
        new Element(ELEMENTS::EMPTY, 0.0f);
    Element::elements[ELEMENTS::SAND] =
        new Sand(ELEMENTS::SAND, 1.0f);
}

int init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Unable to initialize SDL" << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Falling Sand", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 400, 300, SDL_WINDOW_RESIZABLE);
    surface = SDL_GetWindowSurface(window);

    if (surface->format->BitsPerPixel != 32) {
        std::cout << "Not a 32-bit surface" << std::endl;
        return 1;
    }

    renderer = Renderer(window, &world, &mainCamera);
    initInputHandler();
    initElements();

    /*
    for (int x = -10; x < 10; x++) {
        for (int y = -10; y < 10; y++) {
            world.createEmptyChunk(x, y);
        }
    }
    */
    world.createEmptyChunk(0, 0);
    //world.createEmptyChunk(-1, -1);

    return 0;
}

void pollEvents()
{
    while(SDL_PollEvent(&event)) {
        inputHandler.handleEvent(event);
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
        }
    }
}

void draw()
{
    renderer.getSurface();
    surface = SDL_GetWindowSurface(window);

    bool shouldLock = SDL_MUSTLOCK(surface);
    if (shouldLock) { SDL_LockSurface(surface); }
    // Begin drawing

    SDL_FillRect(surface, nullptr, 0);
    renderer.draw();

    // Stop drawing
    if (shouldLock) { SDL_UnlockSurface(surface); }
    SDL_UpdateWindowSurface(window);
}

void updateTimers()
{
    // Calculate deltaTime (wtf chrono)
    float epochTime = (float)ch::duration_cast<ch::milliseconds>(
        ch::system_clock::now().time_since_epoch()).count();

    // frameTime is the frameTimers - epochTime
    frameTime = (epochTime - (float)ch::duration_cast<ch::milliseconds>(
        frameTimer.time_since_epoch()).count()) / 1000.0f;

    deltaTime = timeSlept + frameTime;

    // Put main thread to sleep so fps doesn't exceed refreshRateCap
    if (frameTime < refreshRateCap) {
        timeSlept = (refreshRateCap - frameTime);
        std::this_thread::sleep_for(ch::milliseconds((int)(timeSlept * 1000.0f)));
    }

    frameTimer = ch::system_clock::now();
}

void mainloop()
{
    pollEvents();
    inputHandler.update();

    for (auto& chunk : (*world.getChunks())) {
        CellAutomaton updateTest(&world, chunk.first.first, chunk.first.second);
    }

    //CellAutomaton updateTest(&world, 0, 0);
    draw();
    updateTimers();
}

void quit()
{
    for (int i = 0; i < ELEMENTS::ENUM_SIZE; i++) {
        delete Element::elements[i];
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    int error_code = init();
    if (error_code != 0) {
        return error_code;
    }

    running = true;
    while(running) {
        mainloop();
    }

    quit();
    return 0;
}