/*
 * adt_mesh.hpp
 *
 *  Created on: Oct 27, 2012
 *      Author: pferdone
 */

#ifndef ADT_MESH_HPP_
#define ADT_MESH_HPP_

#include <iostream>
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <SMesh.h>
#include <SMeshBuffer.h>
#include <S3DVertex.h>
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <IMeshSceneNode.h>
#include <IImage.h>
#include <ITexture.h>
#include <squish.h>
#include <vector3d.h>
#include "chunk_helper.hpp"
#include "chunk_structures.hpp"
#include "terrain_constants.hpp"
#include "blp_structures.hpp"
#include "mpqutil.hpp"

using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;

typedef std::vector<SMesh*> SMeshVector_t;
typedef std::vector<IMeshSceneNode*> MeshNodeVector_t;
typedef std::list<std::string> FilenameList_t;
typedef std::vector<ITexture*> TextureVector_t;

const unsigned int USE_ALPHA_MAP = 0x100;
const unsigned int ALPHA_MAP_COMPRESSED = 0x200;

/**
 * ADT mesh create terrain geometry from a MCNK list and adds it to
 * the Irrlicht scene manager.
 */
class ADTMesh_c
{
  public:
    ADTMesh_c(ISceneManager *smgr, IVideoDriver *driver, MPQUtil_c *mpq_util);
    virtual ~ADTMesh_c();

    /**
     * Initializes terrain through MCNK chunk list.
     * \param list MCNK chunk list
     */
    void initTerrain(const ChunkList_t &list);

    bool initTerrainTextures(const ChunkMap_t &tex0);
    /**
     * Returns bounding box. Not 100% accurate.
     */
    const aabbox3df& getBoundingBox() const;

  private:
    /**
     * Adds a map chunk (its geometry) to a mesh.
     * \param mesh mesh to add map chunk to
     * \param info MCNK chunk info
     */
    void addMapChunk(SMesh *mesh, const ChunkInfo_s &info);
    /**
     * This function generates a bounding box based on the first and last
     * chunk position in a chunk list. This method is not 100% accurate.
     * \param list chunk list
     */
    void createBoundingBox(const ChunkList_t &list);

    void getTextureList(ChunkInfo_s *mtex_info, FilenameList_t *list);

    void loadTexture(const std::string &filename);

    void decompressAlphaMap(uint8_t *in, ARGB8_s *out, int index);

    ISceneManager *_smgr;
    IVideoDriver *_driver;
    MPQUtil_c *_mpq_util;
    SMeshVector_t _mesh_vector;
    MeshNodeVector_t _mesh_node_vector;
    TextureVector_t _texture_vector;
    aabbox3df _bounding_box;

    static uint32_t _alpha_map_counter;
};

#endif /* ADT_MESH_HPP_ */
