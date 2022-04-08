#pragma once

#include <cstdlib>

class Chunk
{
public:
    static const int size = 16;
    static const int cellsLength = size * size;

    Chunk();
    Chunk(int _x, int _y);
    ~Chunk();

    int getX();
    int getY();

private:
    // Chunk coordinates
    int x, y;

    // 16 x 16 cells
    // Temp - true if sand, false if nothing
    bool cells[cellsLength];
};