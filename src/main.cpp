/*
 * main.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */


#include "chunk.hpp"
#include "mpqutil.hpp"
#include <fstream>
#include "chunk_structures.hpp"


typedef std::multimap<std::string, Chunk_s*> ChunkMap_t;
typedef std::pair<std::string, Chunk_s*> ChunkPair_t;

void parse_buffer(Buffer_t::const_iterator begin, Buffer_t::const_iterator end, ChunkMap_t *chunk_map);
bool find_first_chunk(const std::string &magic, const ChunkMap_t &map, Chunk_s **chunk);
void save_buffer_to_file(const std::string &filename, const Buffer_t &buffer);

int main(int argc, char **argv)
{
  MPQUtil_c mpq_util;

  Buffer_t file_buffer;
  mpq_util.openArchive("expansion1.MPQ");
  mpq_util.openArchive("expansion2.MPQ");
  mpq_util.readFile("world\\maps\\Northrend\\Northrend_22_27.adt", &file_buffer);

  save_buffer_to_file("Northrend_22_27.adt", file_buffer);

  ChunkMap_t adt_chunk_map;
  parse_buffer(file_buffer.begin(), file_buffer.end(), &adt_chunk_map);

  /*Chunk_s *mhdr_chunk;
  find_first_chunk("RDHM", adt_chunk_map, &mhdr_chunk);
  MHDRstruct_s *mhdr = reinterpret_cast<MHDRstruct_s*>(mhdr_chunk->buffer.data());

  Chunk_s *mcnk_chunk;
  find_first_chunk("KNCM", adt_chunk_map, &mcnk_chunk);*/

  Chunk_s *mver_chunk;
  find_first_chunk("REVM", adt_chunk_map, &mver_chunk);

  MVERstruct_s *mver = reinterpret_cast<MVERstruct_s*>(mver_chunk->buffer.data());
  /*MCNKstruct_s *mcnk = reinterpret_cast<MCNKstruct_s*>(mcnk_chunk->buffer.data());
  MCVTstruct_s *mcvt = reinterpret_cast<MCVTstruct_s*>(mcnk_chunk->buffer.data()+mcnk->mcvt_off);
  MCNRstruct_s *mcnr = reinterpret_cast<MCNRstruct_s*>(mcnk_chunk->buffer.data()+mcnk->mcnr_off);
  MCLYstruct_s *mcly = reinterpret_cast<MCLYstruct_s*>(mcnk_chunk->buffer.data()+mcnk->mcly_off);*/

  std::cout << "MVER version: " << mver->version << std::endl;

  return 0;
}

void parse_buffer(Buffer_t::const_iterator begin, Buffer_t::const_iterator end, ChunkMap_t *chunk_map)
{
  Buffer_t::const_iterator iter = begin;
  std::size_t ci_size = sizeof(ChunkInfo_s);
  while (iter!=end) {
    Chunk_s *chunk = new Chunk_s();

    memcpy(&chunk->info, &(*iter), ci_size);
    chunk->buffer.resize(chunk->info.size);
    memcpy(chunk->buffer.data(), &(*(iter+ci_size)), chunk->info.size);

    iter+=chunk->info.size+ci_size;

    std::string magic(chunk->info.id, chunk->info.id+4);
    chunk_map->insert(ChunkPair_t(magic, chunk));
  }
}

bool find_first_chunk(const std::string &magic, const ChunkMap_t &map, Chunk_s **chunk)
{
  ChunkMap_t::const_iterator found = map.find(magic);
  if (found!=map.end()) {
    *chunk = found->second;
    return true;
  }

  return false;
}

void save_buffer_to_file(const std::string &filename, const Buffer_t &buffer)
{
  std::ofstream os(filename.c_str());
  os.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}
