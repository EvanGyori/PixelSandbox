#pragma once

#include <cstdint>
#include <cstdlib>

#include "Settings.h"

struct Cell
{
    ty::ElementId element;
    uint32_t color;
    float vx, vy;
    // value with range (-1, 1) exclusive. It keeps the progress of movement
    // from previous updates to allow for slower movement.
    float progressX, progressY;

    Cell();
};
