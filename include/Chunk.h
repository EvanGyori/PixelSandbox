#pragma once

#include <cstdlib>
#include <cmath>

#include "Cell.h"

struct Chunk
{
public:
    static const int size = 16;
    static const int cellsLength = size * size;

    // Chunk coordinates
    int x, y;

    // 16 x 16 cells
    // Temp - true if sand, false if nothing
    Cell cells[cellsLength];

    Chunk();
    Chunk(int _x, int _y);
};