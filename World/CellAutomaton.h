#pragma once

#include "Settings.h"
#include "World.h"
#include "Element.h"

// Can be used across multiple threads and only updates
// the specified chunk, make sure to use seperate 
// instances of this for each thread and to
// delete them afterwards
class CellAutomaton
{
public:
    static World* world;

    // Give x and y of chunk, updates the chunk upon initilization
    // delete this instance afterwards
    CellAutomaton(int _cx, int _cy);
    ~CellAutomaton();

    static void initChunk(Chunk& chunk);

    // World coordinates
    static void setCell(Cell cell, int x, int y);

    // x and y must be between -8 and 24
    Cell* getCell(int x, int y);

    // If no chunk exists here, create an empty chunk
    static Chunk* getChunk(int x, int y);

    // x and y must be between -8 and 24
    void swapCells(int x1, int y1, int x2, int y2);

    // Checks if the cell is within the chunk
    bool inBounds(int x, int y);

private:
    // The chunk being updated
    Chunk* chunk;
    
    // Coordinates of chunk being updated
    int cx, cy;

    // first - source, the original index of the cell that
    // is currently in this position/index
    // second - destination, the new position/index of the cell
    // that was originally here
    // Both source and destination start as the index
    std::pair<int, int> changes[Chunk::cellsLength];
};