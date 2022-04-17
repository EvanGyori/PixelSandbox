#include "World.h"

World::World()
{
}

World::~World()
{
}

ty::ChunkMap* World::getChunks()
{
    return &chunks;
}

void World::createEmptyChunk(int x, int y)
{
    chunks[std::make_pair(x, y)] = Chunk(x, y);
}