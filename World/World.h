#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

#include "Settings.h"
#include "Chunk.h"
#include "HashPair.h"

namespace ty
{
    typedef std::unordered_map
    <std::pair<int, int>, Chunk, HashPair> ChunkMap;
};

class World
{
public:
    World();
    ~World();

    ty::ChunkMap* getChunks();

    void createEmptyChunk(int x, int y);

private:
    ty::ChunkMap chunks;
    //std::vector<Chunk> chunks;
};