
#include "Chunk.h"

Chunk::Chunk()
{
}

Chunk::Chunk(int _x, int _y)
{
    x = _x;
    y = _y;
    for (int i = 0; i < cellsLength; i++) {
    	cells[i].element = 0;
    	cells[i].color = 0;
    	cells[i].vx = 0.0f;
    	cells[i].vy = 0.0f;
    	cells[i].progressX = 0.0f;
    	cells[i].progressY = 0.0f;
    }
}
