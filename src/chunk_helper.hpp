/*
 * chunk_helper.hpp
 *
 *  Created on: Oct 27, 2012
 *      Author: pferdone
 */

#ifndef CHUNK_HELPER_HPP_
#define CHUNK_HELPER_HPP_

#include <fstream>
#include "chunk_structures.hpp"


// chunk related containers
typedef std::multimap<std::string, ChunkInfo_s*> ChunkMap_t;
typedef std::pair<std::string, ChunkInfo_s*> ChunkPair_t;
typedef std::list<ChunkInfo_s*> ChunkList_t;


/**
 * Takes a buffer (represented by begin and end iterator) and parses all chunks that are contained within.
 * \param begin (in) iterator pointing to the start of the buffer
 * \param end (in) iterator pointing to the end of the buffer
 * \param chunk_map (out) map that receives all chunks contained in a buffer
 * \param skip_mcnk (in) wether or not to skip MCNK chunks, necessary for obj0, obj1, tex0 and tex1 files
 */
void parse_buffer(Buffer_t::const_iterator begin, Buffer_t::const_iterator end, ChunkMap_t *chunk_map, bool skip_mcnk=false)
{
  Buffer_t::const_iterator iter = begin;
  std::size_t ch_size = sizeof(ChunkHeader_s);

  while (iter!=end) {
    ChunkInfo_s *chunk_info = new ChunkInfo_s();
    const ChunkHeader_s *chunk_header = reinterpret_cast<const ChunkHeader_s*>(&(*iter));

    // fill chunk info
    std::string magic(chunk_header->id, chunk_header->id+4);

    if (skip_mcnk && (magic == "KNCM")) {
      iter+=ch_size;
    } else {
      chunk_info->id = magic;
      chunk_info->offset = std::distance(begin, iter);
      chunk_info->size = chunk_header->size;
      chunk_info->buffer.insert(chunk_info->buffer.begin(), iter+ch_size, iter+ch_size+chunk_header->size);

      chunk_map->insert(ChunkPair_t(magic, chunk_info));

      // advance iterator
      iter+=ch_size+chunk_header->size;
    }
  }
}

/**
 * Simply saves a buffer to a file.
 * \param filename (in) filename
 * \param buffer (in) buffer to write to disk
 */
void save_buffer_to_file(const std::string &filename, const Buffer_t &buffer)
{
  std::ofstream os(filename.c_str());
  os.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

/**
 * Finds the first chunk with a specific id inside a ChunkMap.
 * \param magic (in) id or magic number as a string
 * \param map (in) chunk map to look for the specific id
 * \param chunk_info (out) chunk information and data
 * \return Returns true if chunk can be found, otherwise false.
 */
bool find_first_chunk(const std::string &magic, const ChunkMap_t &map, ChunkInfo_s **chunk_info)
{
  ChunkMap_t::const_iterator found = map.find(magic);
  if (found!=map.end()) {
    *chunk_info = found->second;
    return true;
  }

  return false;
}

/**
 * Find all chunks with a given id and return them as a list.
 * \param magic (in) id or magic number as a string
 * \param map (in) chunk map to look for the specific id
 * \param chunk_list (out) list which receives all chunks matching the given id
 */
void find_all_chunks(const std::string &magic, const ChunkMap_t &map, ChunkList_t *chunk_list)
{
  for (ChunkMap_t::const_iterator iter=map.begin(); iter!=map.end(); ++iter) {
    if (iter->second->id == magic) {
      chunk_list->push_back(iter->second);
    }
  }
}


#endif /* CHUNK_HELPER_HPP_ */
