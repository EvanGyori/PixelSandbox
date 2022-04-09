#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

#include "Chunk.h"
#include "HashPair.h"

typedef std::unordered_map<std::pair<int, int>, Chunk, HashPair> ChunkMap;

class World
{
public:
    World();
    ~World();

    ChunkMap* getChunks();

    void createEmptyChunk(int x, int y);

private:
    ChunkMap chunks;
    //std::vector<Chunk> chunks;
};