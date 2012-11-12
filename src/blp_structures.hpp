/*
 * blp_structures.hpp
 *
 *  Created on: Nov 11, 2012
 *      Author: pferdone
 */

#ifndef BLP_STRUCTURES_HPP_
#define BLP_STRUCTURES_HPP_

enum BLP_TYPE
{
  BLP_TYPE_JPEG_COMPRESSION = 0,
  BLP_TYPE_DX_OR_UNCOMPRESSED = 1
};

enum BLP_ENCODING
{
  BLP_ENC_UNCOMPRESSED = 1,
  BLP_ENC_DX_COMPRESSION = 2
};

enum BLP_ALPHA_DEPTH
{
  BLP_AD_NO_ALPHA = 0,
  BLP_AD_1_BIT = 1,
  BLP_AD_4_BIT = 4,
  BLP_AD_8_BIT = 8
};

enum BLP_ALPHA_ENCODING
{
  BLP_AE_DXT1 = 0,
  BLP_AE_DXT23 = 1,
  BLP_AE_DXT45 = 7
};

enum BLP_MIPS
{
  BLP_MIPS_NO = 0,
  BLP_MIPS_YES = 1,
};

#pragma pack(push, 1)
struct ARGB8_s
{
  union {
    uint32_t color;
    struct {
    uint8_t r,g,b,a;
    };
    uint8_t c[4];
  };
};
#pragma pack(pop)

struct BLP2Header_s
{
  uint32_t id; // Always 'BLP2'
  uint32_t type;
  uint8_t encoding;
  uint8_t alpha_depth;
  uint8_t alpha_encoding;
  uint8_t has_mips;
  uint32_t width;
  uint32_t height;
  uint32_t offsets[16];
  uint32_t lengths[16];
  ARGB8_s  palette[256];
};

#endif /* BLP_STRUCTURES_HPP_ */
