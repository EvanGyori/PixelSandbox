/*
Debug Terminal Command:
g++ -o debug -Wall -I ./include/ src/main.cpp src/Renderer.cpp src/Camera.cpp src/InputHandler.cpp -lSDL2

*/


#include "SDL2/SDL.h"
#include <iostream>
#include <chrono>
#include <thread>

#include "Renderer.h"
#include "InputHandler.h"

#define DEBUG


namespace ch = std::chrono;
typedef ch::system_clock::time_point ch_time;

ch_time frameTimer;

// minimum time per frame in seconds
const float refreshRateCap = 0.01;

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

int init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Unable to initialize SDL" << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Falling Sand", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_RESIZABLE);
    surface = SDL_GetWindowSurface(window);

    if (surface->format->BitsPerPixel != 32) {
        std::cout << "Not a 32-bit surface" << std::endl;
        return 1;
    }

    renderer = Renderer(window, &mainCamera);
    initInputHandler();

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
    renderer.drawCell(0xFFFFFF, -1, -1);
    renderer.drawCell(0xFFFFFF, 0, 0);

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
    draw();
    updateTimers();
}

void quit()
{
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