#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::addEvent(std::string id, eventFunc func)
{
    events.insert({id, func});
}

void InputHandler::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEWHEEL) {
        events.at("zoom")();
    }
}

void InputHandler::update()
{
    const uint8_t *keysState = SDL_GetKeyboardState(nullptr);

    // Movement
    if (keysState[SDL_SCANCODE_W]) {
        events.at("moveUp")();
    }
    if (keysState[SDL_SCANCODE_S]) {
        events.at("moveDown")();
    }
    if (keysState[SDL_SCANCODE_D]) {
        events.at("moveRight")();
    }
    if (keysState[SDL_SCANCODE_A]) {
        events.at("moveLeft")();
    }
}