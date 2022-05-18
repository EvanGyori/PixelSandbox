#include "CellAutomaton.h"

World* CellAutomaton::world = nullptr;

CellAutomaton::CellAutomaton(int _cx, int _cy)
{
    cx = _cx;
    cy = _cy;

    auto pair = std::make_pair(cx, cy);

    #if defined(DEBUG)
    if (world->getChunks()->find(pair) == world->getChunks()->end()) {
        std::cout << "ERROR: attempting to update a chunk that "
        << "doesn't exist at (" << cx << ", " << cy << ")"
        << std::endl;
    }
    #endif

    chunk = &world->getChunks()->at(pair);

    // This method may be overly complicated and over engineered
    // but its the only flawless way I could find that works
    // with chunks

    // Purpose: Store a list of the cells so we can loop
    // through them and change their position without
    // accidently updating a cell twice

    for (int i = 0; i < Chunk::cellsLength; i++) {
        changes[i].first = i;
        changes[i].second = i;
    }

    // To get the cell of the current iteration in a loop,
    // use the destination as an index in the chunk
    // cell array
    for (int x = 0; x < Chunk::size; x++)
    for (int y = 0; y < Chunk::size; y++) {
        int index = x + y * Chunk::size;
        // If destination is -1, the cell that was originally here
        // is now in a different chunk and can be skipped
        if (changes[index].second != -1) {
            Cell* cell = &chunk->cells[changes[index].second];
            Element::elements[cell->element]->updateCell(
                (*this), (*cell), x, y);
        }
    }
}

CellAutomaton::~CellAutomaton()
{
}

void CellAutomaton::initChunk(Chunk& chunk)
{
    for (int i = 0; i < Chunk::cellsLength; i++) {
        Element::elements[chunk.cells[i].element]->
        initCell(chunk.cells[i]);
    }
}

Cell* CellAutomaton::setCell(Cell cell, int x, int y)
{
    int cx = x/Chunk::size;
    int cy = y/Chunk::size;

    x %= Chunk::size;
    y %= Chunk::size;
    
    Element::elements[cell.element]->initCell(cell);
    getChunk(cx, cy)->cells[x + y * Chunk::size] = cell;
}

Cell* CellAutomaton::getCell(int x, int y)
{
    #if defined(DEBUG)
    if (x < -Chunk::size/2 || y < -Chunk::size/2 ||
    x >= Chunk::size * 3/2 || y >= Chunk::size * 3/2) {
        std::cout << "ERROR: attempting to edit cells out of "
        << "the thread's bounds which may cause multi-thread "
        << "overlapping" << std::endl;
    }
    #endif

    // Find chunk the cell is in, again make sure the values do
    // not interfere with other threads
    int chunkX = cx;
    int chunkY = cy;

    if (x < 0) {
        x += Chunk::size;
        chunkX--;
    } else if (x >= Chunk::size) {
        x -= Chunk::size;
        chunkX++;
    }

    if (y < 0) {
        y += Chunk::size;
        chunkY--;
    } else if (y >= Chunk::size) {
        y -= Chunk::size;
        chunkY++;
    }

    // TODO find way to create chunks in multithreading

    return &getChunk(chunkX, chunkY)->cells[x + y * Chunk::size];
}

Chunk* CellAutomaton::getChunk(int x, int y)
{
    ty::ChunkMap* chunks = world->getChunks();
    auto pair = std::make_pair(x, y);

    if (chunks->find(pair) != chunks->end()) {
        return &chunks->at(pair);
    } else {
        world->createEmptyChunk(x, y);
        initChunk(chunks->at(pair));
        return &chunks->at(pair);
    }

}

void CellAutomaton::swapCells(int x1, int y1, int x2, int y2)
{
    Cell* cell1 = getCell(x1, y1);
    Cell* cell2 = getCell(x2, y2);

    Cell temp = (*cell1);
    (*cell1) = (*cell2);
    (*cell2) = temp;

    // If a cell swaps with a cell outside the chunk,
    // set the destination of the source to -1 then set the source
    // to -1 because the cell in this is from outside
    // and the orginal cell's destination is outside

    if (inBounds(x1, y1) && inBounds(x2, y2)) {
        // swap each others source, then swap the destinations of
        // the original source with the new found source
        int index1 = x1 + y1 * Chunk::size;
        int index2 = x2 + y2 * Chunk::size;
        
        // Swap destinations of sources
        auto& dest1 = changes[changes[index1].first].second;
        auto& dest2 = changes[changes[index2].first].second;
        int temp = dest1;
        dest1 = dest2;
        dest2 = temp;

        // Swap sources
        temp = changes[index1].first;
        changes[index1].first = changes[index2].first;
        changes[index2].first = temp;
    } else if (!inBounds(x1, y1) && inBounds(x2, y2)) {
        int index2 = x2 + y2 * Chunk::size;
        changes[changes[index2].first].second = -1;
        changes[index2].first = -1;
    } else if (!inBounds(x2, y2) && inBounds(x1, y1)) {
        int index1 = x1 + y1 * Chunk::size;
        changes[changes[index1].first].second = -1;
        changes[index1].first = -1;
    }
}

bool CellAutomaton::inBounds(int x, int y)
{
    return !(x < 0 || y < 0
        || x >= Chunk::size || y >= Chunk::size);
}