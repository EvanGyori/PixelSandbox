#include "Chunk.h"

Chunk::Chunk()
{
}

Chunk::Chunk(int _x, int _y)
{
    x = _x;
    y = _y;

    for (int i = 0; i < cellsLength; i++) {
        uint32_t color = rand() * 131000; //pow(rand() % 256, 4);
        cells[i] = Cell(color);
    }
}