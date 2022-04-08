#include "Chunk.h"

Chunk::Chunk()
{
}

Chunk::Chunk(int _x, int _y)
{
    x = _x;
    y = _y;

    for (int i = 0; i < cellsLength; i++) {
        cells[i] = rand() % 2;
    }
}

Chunk::~Chunk()
{
}

int Chunk::getX()
{
    return x;
}

int Chunk::getY()
{
    return y;
}