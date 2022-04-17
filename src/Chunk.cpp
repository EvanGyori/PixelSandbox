#include "Chunk.h"

Chunk::Chunk()
{
}

Chunk::Chunk(int _x, int _y)
{
    x = _x;
    y = _y;

    /*
    cells[0].element = ELEMENTS::SAND;
    for (int i = 0; i < cellsLength; i++) {
        Element::elements[cells[i].element]->initCell(cells[i]);
    }
    */
}