#include "World.h"

World::World()
{
}

World::~World()
{
}

std::vector<Chunk>* World::getChunks()
{
    return &chunks;
}

void World::createEmptyChunk(int x, int y)
{
    chunks.push_back(Chunk(x ,y));
}