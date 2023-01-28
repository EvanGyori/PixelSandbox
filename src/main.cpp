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
const float refreshRateCap = 0.001f;
const float updateElementsRate = 0.001f;

// how long since the last elements update
float elementsTimer;

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

ty::ElementId elementBrush;
const int brushSize = 5;

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

    inputHandler.addEvent("paintBrush", [&]() {
        int x = event.button.x;
        int y = event.button.y;
        int sw, sh;
        SDL_GetWindowSize(window, &sw, &sh);
        mainCamera.convertToWorld(x, y, sw, sh);

        //CellAutomaton::setCell()
    });
}

void initElements()
{
    elementBrush = ELEMENTS::EMPTY;
    Element::elements[ELEMENTS::EMPTY] =
        new Element(ELEMENTS::EMPTY, false, 0.0f);
    Element::elements[ELEMENTS::SAND] =
        new FallingElement(ELEMENTS::SAND, true, 1.0f);
    Element::elements[ELEMENTS::STONE] =
        new StoneElement(ELEMENTS::STONE, true, 1.0f);
}

int init()
{
    CellAutomaton::world = &world;
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
    for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 5; x++) {
			world.createEmptyChunk(x, y);
			Chunk& chunk = world.getChunks()->at(std::make_pair(x, y));
			for (int i = 0; i < Chunk::cellsLength; i++) {
				chunk.cells[i].element = rand() % 2;
			}
			for (int i = 240; i < 256 && y == 9; i++) {
				chunk.cells[i].element = 2;
			}
			CellAutomaton::initChunk(chunk);
			//world.createEmptyChunk(-1, -1);
		}
	}
	
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
    elementsTimer += deltaTime;

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

	if (elementsTimer >= updateElementsRate) {
		elementsTimer = 0.0f;
		for (auto& chunk : (*world.getChunks())) {
		    CellAutomaton updateTest(chunk.first.first, chunk.first.second);
		}
	}

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
