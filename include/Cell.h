#pragma once

#include <cstdint>
#include <cstdlib>

struct Cell
{
    uint32_t color;

    Cell()
    {
    }

    Cell(uint32_t _color)
    {
        color = _color;
    }
};