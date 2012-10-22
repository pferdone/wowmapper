/*
 * chunk.hpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */

#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include <stdint.h>
#include <vector>

typedef std::vector<uint8_t> Buffer_t;

/**
 * Struct to read receive information all chunks share.
 */
#pragma pack(push, 1)
struct ChunkInfo_s
{
  uint8_t id[4];
  uint32_t size;
};
#pragma pack(pop)

struct Chunk_s
{
  ChunkInfo_s info;
  Buffer_t buffer;
};


#endif /* CHUNK_HPP_ */
