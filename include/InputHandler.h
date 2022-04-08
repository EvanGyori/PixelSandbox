#pragma once

#include "SDL2/SDL.h"
#include <unordered_map>
#include <functional>
#include <string>

typedef std::function<void()> eventFunc;

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();

    void addEvent(std::string id, eventFunc func);

    void handleEvent(SDL_Event& event);

    void update();

private:
    std::unordered_map<std::string, eventFunc> events;
};