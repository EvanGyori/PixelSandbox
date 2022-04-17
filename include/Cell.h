#pragma once

#include <cstdint>
#include <cstdlib>

#include "Settings.h"

struct Cell
{
    ty::ElementId element;
    uint32_t color;

    Cell();
};