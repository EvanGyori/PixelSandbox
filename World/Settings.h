#pragma once

#include <cstdint>

#define DEBUG

// Typedefs
namespace ty
{
    // typedef made in world class instead
    //typedef std::unordered_map<std::pair<int, int>, Chunk, HashPair> ChunkMap;
    typedef uint16_t ElementId;
    typedef uint8_t CellIndex;
};