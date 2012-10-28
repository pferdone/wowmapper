/*
 * main.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */


#include <iostream>
#include "mpqutil.hpp"
#include "chunk_helper.hpp"
#include "terrain_constants.hpp"

#include <irrlicht/irrlicht.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

std::vector<glm::vec3> vertices;

void get_terrain(const ChunkInfo_s &mcnk_info, SMeshBuffer *mesh_buffer);

/**
 * Main function.
 */
int main(int argc, char **argv)
{
  MPQUtil_c mpq_util;
  //mpq_util.openArchive("expansion1.MPQ");
  mpq_util.openArchive("expansion2.MPQ");

  ChunkMap_t adt_map, obj0_map, obj1_map, tex0_map, tex1_map;
  Buffer_t file_buffer;

  mpq_util.readFile("world\\maps\\Northrend\\Northrend_22_28.adt", &file_buffer);
  parse_buffer(file_buffer.begin(), file_buffer.end(), &adt_map);


  IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, true, false, 0);
  device->setWindowCaption(L"wowmapper");
  ISceneManager *smgr = device->getSceneManager();

  ChunkList_t mcnk_list;
  find_all_chunks("KNCM", adt_map, &mcnk_list);
  vector3df pos;

  for (ChunkList_t::const_iterator iter=mcnk_list.begin(); iter!=mcnk_list.end(); ++iter) {
    // create mesh
    SMesh *mesh = new SMesh();
    SMeshBuffer *mesh_buffer = new SMeshBuffer();
    mesh->addMeshBuffer(mesh_buffer);
    IMeshSceneNode *mesh_node = smgr->addMeshSceneNode(mesh);
    mesh_node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

    const ChunkInfo_s *mcnk_info = reinterpret_cast<const ChunkInfo_s*>(*iter);
    const MCNKstruct_s *mcnk = mcnk_info->get<MCNKstruct_s>();
    get_terrain(*mcnk_info, mesh_buffer);

    pos.set(-mcnk->position.y, mcnk->position.z, -mcnk->position.x);
    mesh_node->setPosition(pos);
  }

  //aabbox3d<f32> bbox = smgr->getRootSceneNode()->getBoundingBox();


  ILightSceneNode *light_node = smgr->addLightSceneNode();
  light_node->setPosition(pos);
  ICameraSceneNode *camera_node = smgr->addCameraSceneNodeFPS();
  camera_node->setFarValue(20000.0f);
  camera_node->setPosition(pos);

  IVideoDriver *driver = device->getVideoDriver();

  while(device->run()) {
    driver->beginScene(true, true, SColor(0xffffffff));
    smgr->drawAll();
    driver->endScene();
  }

  device->drop();

  return 0;
}

void get_terrain(const ChunkInfo_s &mcnk_info, SMeshBuffer *mesh_buffer)
{
  const MCNKstruct_s *mcnk = mcnk_info.get<MCNKstruct_s>();
  const MCVTstruct_s *mcvt = mcnk_info.get<MCVTstruct_s>(mcnk->mcvt_off);
  const MCNRstruct_s *mcnr = mcnk_info.get<MCNRstruct_s>(mcnk->mcnr_off);

  vertices.insert(vertices.begin(), VERTICES.begin(), VERTICES.end());

  mesh_buffer->Vertices.set_used(VERTICES.size());
  for (int i=0; i<145; i++) {
    vertices[i].y = mcvt->heights[i];
    mesh_buffer->Vertices[i].Pos.set(vertices[i].x, vertices[i].y, vertices[i].z);
    mesh_buffer->Vertices[i].Normal.set(mcnr->normals[i].x/127.f, mcnr->normals[i].y/127.f, mcnr->normals[i].z/127.f);
    mesh_buffer->Vertices[i].TCoords.set(TEXCOORDS[i].x, TEXCOORDS[i].y);
    mesh_buffer->Vertices[i].Color.set(0xcccccc);
  }

  mesh_buffer->Indices.set_used(INDICES.size());
  for (int i=0; i<768; i++) {
    mesh_buffer->Indices[i] = INDICES[i];
  }

  mesh_buffer->recalculateBoundingBox();
}
