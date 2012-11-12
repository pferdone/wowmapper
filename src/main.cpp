/*
 * main.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: pferdone
 */


#include <iostream>
#include <irrlicht/irrlicht.h>
#include <IGPUProgrammingServices.h>
#include "irrlicht/shader_callback.hpp"
#include "mpqutil.hpp"
#include "chunk_helper.hpp"
#include "adt_mesh.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void initIrrlicht(IrrlichtDevice **device, IVideoDriver **driver, ISceneManager **smgr);
void createLight(ISceneManager *smgr, IVideoDriver *driver, const vector3df &pos);
void createCamera(ISceneManager *smgr, const vector3df &pos);

/**
 * Main function.
 */
int main(int argc, char **argv)
{
  IrrlichtDevice *device = 0;
  IVideoDriver *driver = 0;
  ISceneManager *smgr = 0;
  initIrrlicht(&device, &driver, &smgr);

  MPQUtil_c mpq_util;
  Buffer_t wdt_file_buffer;
  ChunkMap_t wdt_map;

  // read WDT from MPQ
  mpq_util.openArchive("data/texture.MPQ");
  mpq_util.openArchive("data/expansion1.MPQ");
  mpq_util.openArchive("data/expansion2.MPQ");
  mpq_util.openArchive("data/expansion3.MPQ");
  mpq_util.openArchive("data/expansion4.MPQ");
  mpq_util.readFile("World\\Maps\\Northrend\\Northrend.wdt", &wdt_file_buffer);
  parse_buffer(wdt_file_buffer.begin(), wdt_file_buffer.end(), &wdt_map);

  // get areas
  ChunkInfo_s *chunk_info;
  find_first_chunk("NIAM", wdt_map, &chunk_info);
  const MAINstruct_s *main = chunk_info->get<MAINstruct_s>();

  std::string area("World\\Maps\\Northrend\\Northrend");
  aabbox3df bb;

  int i=0;
  for (int y=25;y<28;y++) {
    for (int x=25;x<28;x++) {
      if (main->info[y*64+x].flags & 0x1) {
        //char adt_fn[128], tex0_fn[128];
        //sprintf(adt_fn, "%s_%02d_%02d.adt", area.c_str(), x, y);

//        ChunkMap_t adt_map, tex0_map;
//        Buffer_t adt_fb, tex0_fb;
//        // read ADT from MPQ
//        mpq_util.readFile(adt_fn, &adt_fb);
//        parse_buffer(adt_fb.begin(), adt_fb.end(), &adt_map);
//        mpq_util.readFile(tex0_fn, &tex0_fb);
//        parse_buffer(tex0_fb.begin(), tex0_fb.end(), &tex0_map, true);
//
//        // create mesh from ADT
//        ADTMesh_c *adt_mesh = new ADTMesh_c(smgr);
//        ChunkList_t mcnk_list;
//        find_all_chunks("KNCM", adt_map, &mcnk_list);
//        adt_mesh->initTerrain(mcnk_list);
//        adt_mesh->initTerrainTextures(tex0_map, tex0_map);
//
//        bb = adt_mesh->getBoundingBox();

        char adt_fn[128], tex0_fn[128], tex1_fn[128], obj0_fn[128], obj1_fn[128];
        Buffer_t adt_fb, tex0_fb, tex1_fb, obj0_fb, obj1_fb;
        ChunkMap_t adt_map, tex0_map, tex1_map, obj0_map, obj1_map;

        // create filenames
        sprintf(adt_fn, "%s_%02d_%02d.adt", area.c_str(), x, y);
        sprintf(tex0_fn, "%s_%02d_%02d_tex0.adt", area.c_str(), x, y);
        sprintf(tex1_fn, "%s_%02d_%02d_tex1.adt", area.c_str(), x, y);
        sprintf(obj0_fn, "%s_%02d_%02d_obj0.adt", area.c_str(), x, y);
        sprintf(obj1_fn, "%s_%02d_%02d_obj1.adt", area.c_str(), x, y);

        // read files into buffer
        mpq_util.readFile(adt_fn, &adt_fb);
        mpq_util.readFile(tex0_fn, &tex0_fb);
        //mpq_util.readFile(tex1_fn, &tex1_fb);
        //mpq_util.readFile(obj0_fn, &obj0_fb);
        //mpq_util.readFile(obj1_fn, &obj1_fb);

        // parse chunks
        parse_buffer(adt_fb.begin(), adt_fb.end(), &adt_map);
        parse_buffer(tex0_fb.begin(), tex0_fb.end(), &tex0_map, true);
        //parse_buffer(tex1_fb.begin(), tex1_fb.end(), &tex1_map, true);
        //parse_buffer(obj0_fb.begin(), obj0_fb.end(), &obj0_map, true);
        //parse_buffer(obj1_fb.begin(), obj1_fb.end(), &obj1_map, true);

        ADTMesh_c *adt_mesh = new ADTMesh_c(smgr, driver, &mpq_util);
        ChunkList_t mcnk_list;
        find_all_chunks("KNCM", adt_map, &mcnk_list);
        adt_mesh->initTerrain(mcnk_list);
        adt_mesh->initTerrainTextures(tex0_map);
        bb = adt_mesh->getBoundingBox();


      }
    }
  }

  vector3df pos = bb.getCenter();
  pos.set(pos.X, pos.Y+1500, pos.Z);

  createLight(smgr, driver, pos);
  createCamera(smgr, pos);


  while(device->run()) {
    driver->beginScene(true, true, SColor(0xffA7AFE8));
    smgr->drawAll();
    driver->endScene();
  }

  device->drop();

  return 0;
}

void initIrrlicht(IrrlichtDevice **device, IVideoDriver **driver, ISceneManager **smgr)
{
  *device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, true, false, 0);
  *driver = (*device)->getVideoDriver();
  *smgr = (*device)->getSceneManager();

  (*device)->setWindowCaption(L"wowmapper");

  IGPUProgrammingServices *gpu = (*driver)->getGPUProgrammingServices();
  irr::s32 mat_type0 = gpu->addHighLevelShaderMaterialFromFiles(
    "shader.vert", "vertexMain", EVST_VS_1_1,
    "shader.frag", "pixelMain", EPST_PS_1_1,
    new ShaderCallback_c(), EMT_SOLID, 0, EGSL_DEFAULT
  );
}

void createLight(ISceneManager *smgr, IVideoDriver *driver, const vector3df &pos)
{
  ILightSceneNode *light_node = smgr->addLightSceneNode(0, pos, SColorf(1,1,1,1), 500);
  ISceneNode *bb_node = smgr->addBillboardSceneNode(light_node, dimension2df(60,60));
  bb_node->setMaterialFlag(EMF_LIGHTING, false);
  bb_node->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
  bb_node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
  bb_node->setMaterialTexture(0, driver->getTexture("particlewhite.bmp"));
}

void createCamera(ISceneManager *smgr, const vector3df &pos)
{
  ICameraSceneNode *camera_node = smgr->addCameraSceneNodeFPS();
  camera_node->setFarValue(20000.0f);
  camera_node->setPosition(pos);
}
