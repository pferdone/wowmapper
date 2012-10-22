/*
 * chunk_structures.hpp
 *
 *  Created on: Oct 21, 2012
 *      Author: pferdone
 */

#ifndef CHUNK_STRUCTURES_HPP_
#define CHUNK_STRUCTURES_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>

typedef uint32_t Offset_t;

#pragma pack(push, 1)
struct MVERstruct_s
{
  uint32_t version;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MHDRstruct_s
{
  uint32_t flags;
  Offset_t mcin_off;
  Offset_t mtex_off;
  Offset_t mmdx_off;
  Offset_t mmid_off;
  Offset_t mwmo_off;
  Offset_t mwid_off;
  Offset_t mddf_off;
  Offset_t modf_off;
  Offset_t mfbo_off;
  Offset_t mh2o_off;
  Offset_t mtxf_off;
  uint32_t unused[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MCNKstruct_s
{
  uint32_t flags;
  uint32_t index_x, index_y;
  uint32_t num_layers;
  uint32_t num_doodad_refs;
  Offset_t mcvt_off;
  Offset_t mcnr_off;
  Offset_t mcly_off;
  Offset_t mcrf_off;
  Offset_t mcal_off;
  uint32_t alpha_size;
  Offset_t mcsh_off;
  uint32_t shadow_size;
  uint32_t area_id;
  uint32_t num_map_obj_refs;
  uint32_t holes;
  uint32_t low_quality_texture_map[4];
  uint32_t pred_tex;
  uint32_t no_effect_doodad;
  Offset_t mcse_off;
  uint32_t num_sound_emitters;
  Offset_t mclq_off;
  uint32_t liquid_size;
  glm::vec3 position;
  Offset_t mccv_off;
  Offset_t mclv_off;
  uint32_t unused;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MCVTstruct_s
{
  glm::vec3 heights[145];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MCNRstruct_s
{
  glm::i8vec3 normals[145];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MCLYstruct_s
{
  uint32_t texture_id;
  uint32_t flags;
  Offset_t off_in_mcal;
  uint16_t effect_id;
  uint16_t padding;
};
#pragma pack(pop)

#endif /* CHUNK_STRUCTURES_HPP_ */
