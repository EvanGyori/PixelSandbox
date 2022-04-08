#pragma once

#include <vector>

#include "Chunk.h"

class World
{
public:
    World();
    ~World();

    std::vector<Chunk>* getChunks();

    void createEmptyChunk(int x, int y);

private:
    std::vector<Chunk> chunks;
};