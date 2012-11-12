/*
 * adt_mesh.cpp
 *
 *  Created on: Oct 27, 2012
 *      Author: pferdone
 */

#include "adt_mesh.hpp"

uint32_t ADTMesh_c::_alpha_map_counter = 0;

ADTMesh_c::ADTMesh_c(ISceneManager *smgr, IVideoDriver *driver, MPQUtil_c *mpq_util)
  : _smgr(smgr),
    _driver(driver),
    _mpq_util(mpq_util)
{

}

ADTMesh_c::~ADTMesh_c()
{
  // TODO clean up
  for (SMeshVector_t::iterator iter=_mesh_vector.begin();
       iter!=_mesh_vector.end();
       ++iter) {
    (*iter)->drop();
  }

  _mesh_vector.clear();
}

void ADTMesh_c::initTerrain(const ChunkList_t& list)
{
  assert("has to have 256 MCNK chunks" && (list.size()==256));

  _mesh_vector.reserve(256);
  _mesh_node_vector.reserve(256);

  vector3df pos;
  for (ChunkList_t::const_iterator iter=list.begin(); iter!=list.end(); ++iter) {
    const ChunkInfo_s *info = *iter;
    const MCNKstruct_s *mcnk = info->get<MCNKstruct_s>();

    // create mesh and add chunk for current MCNK
    SMesh *mesh = new SMesh();
    _mesh_vector.push_back(mesh);
    addMapChunk(mesh, *info);

    // add mesh to scene, disable back face culling and set position
    IMeshSceneNode *mesh_node = _smgr->addMeshSceneNode(mesh);
    _mesh_node_vector.push_back(mesh_node);
    pos.set(-mcnk->position.y, mcnk->position.z, -mcnk->position.x);
    mesh_node->setPosition(pos);
  }

  createBoundingBox(list);
}

bool ADTMesh_c::initTerrainTextures(const ChunkMap_t& tex0)
{
  ChunkList_t tex0_mcnk_list, tex1_mcnk_list;
  find_all_chunks("KNCM", tex0, &tex0_mcnk_list);

  assert("both have to have 256 MCNK chunks" && (tex0_mcnk_list.size()==256));

  ChunkInfo_s *mtex_info=0;
  find_first_chunk("XETM", tex0, &mtex_info);
  FilenameList_t texture_list;
  getTextureList(mtex_info, &texture_list);

  // load all textures
  for (FilenameList_t::iterator iter=texture_list.begin();
       iter!=texture_list.end();
       ++iter) {
    loadTexture(*iter);
  }

  int index=0;
  for (ChunkList_t::iterator iter=tex0_mcnk_list.begin();
       iter!=tex0_mcnk_list.end();
       ++iter, index++) {
    // get chunks
    ChunkInfo_s *mcnk_info = (*iter);
    ChunkInfo_s *mcly_info = 0;
    ChunkInfo_s *mcal_info = 0;

    find_first_chunk("YLCM", mcnk_info->sub_chunks, &mcly_info);
    find_first_chunk("LACM", mcnk_info->sub_chunks, &mcal_info);

    // calculate how many layers the map chunk has
    const MCLYstruct_s *mcly = mcly_info->get<MCLYstruct_s>();
    int num_layers=mcly_info->size/sizeof(MCLYstruct_s);

    ARGB8_s *alpha_map = new ARGB8_s[4096];
    memset(alpha_map, 0, sizeof(ARGB8_s)*4096);

    // create alpha map and set to 0
    for (int i=0; i<num_layers; i++) {
      if (mcly[i].flags & USE_ALPHA_MAP) {
        uint32_t off = mcly[i].off_in_mcal;
        if (mcly[i].flags & ALPHA_MAP_COMPRESSED) {
          decompressAlphaMap(mcal_info->buffer.data()+off, alpha_map, i);
        } else {
          if ((mcal_info->size-off)<4096) {
            std::cout << "break" << std::endl;
            break; }
          const uint8_t *alpha_values = mcal_info->buffer.data()+off;
          for (int j=0; j<4096; j++) {
            alpha_map[j].c[i] = alpha_values[j];
          }
        }
      }

      _mesh_node_vector[index]->setMaterialTexture(i, _texture_vector[mcly[i].texture_id]);
    }

    // create a unique name for our alpha map
    char alpha_map_name[64];
    sprintf(alpha_map_name, "alpha_map_%06d", _alpha_map_counter++);
    IImage *alpha_image = _driver->createImageFromData(ECF_A8R8G8B8, dimension2du(64, 64), alpha_map, true, true);
    ITexture *alpha_texture = _driver->addTexture(alpha_map_name, alpha_image, 0);

    // set material flags for mesh node
    _mesh_node_vector[index]->setMaterialType((E_MATERIAL_TYPE)24);
    _mesh_node_vector[index]->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    _mesh_node_vector[index]->setMaterialFlag(EMF_LIGHTING, false);
    _mesh_node_vector[index]->setMaterialTexture(4, alpha_texture);
  }

  return true;
}

const aabbox3df& ADTMesh_c::getBoundingBox() const
{
  return _bounding_box;
}

void ADTMesh_c::addMapChunk(SMesh* mesh, const ChunkInfo_s &info)
{
  const MCNKstruct_s *mcnk = info.get<MCNKstruct_s>();
  const MCVTstruct_s *mcvt = info.get<MCVTstruct_s>(mcnk->mcvt_off);
  const MCNRstruct_s *mcnr = info.get<MCNRstruct_s>(mcnk->mcnr_off);

  // copy constant vertices
  int num_verts = VERTICES.size();
  std::vector<glm::vec3> vertices;
  vertices.insert(vertices.begin(), VERTICES.begin(), VERTICES.end());

  // create mesh buffer which contains geometry information
  SMeshBuffer *mesh_buffer = new SMeshBuffer();
  mesh->addMeshBuffer(mesh_buffer);
  mesh_buffer->Vertices.set_used(num_verts);

  for (int i=0; i<num_verts; i++) {
    vertices[i].y = mcvt->heights[i];
    S3DVertex &v = mesh_buffer->Vertices[i];
    const glm::i8vec3 *normals = mcnr->normals;

    // what's the point in making vertices x,y,z, but normals are -y,z,-x and
    // even chunk translation is -y,z,-x. i don't get it or i'm just stupid.
    v.Pos.set(vertices[i].x, vertices[i].y, vertices[i].z);
    v.Normal.set(-normals[i].y/127.f, normals[i].z/127.f, -normals[i].x/127.f);
    v.TCoords.set(TEXCOORDS[i].x, TEXCOORDS[i].y);
    v.Color.set(0xffffffff);
  }

  // copy constant indices
  mesh_buffer->Indices.set_used(INDICES.size());
  for (int i=0; i<768; i++) {
    mesh_buffer->Indices[i] = INDICES[i];
  }

  mesh_buffer->recalculateBoundingBox();
  _bounding_box.addInternalBox(mesh_buffer->getBoundingBox());
}

void ADTMesh_c::createBoundingBox(const ChunkList_t& list)
{
  const MCNKstruct_s *mcnk_front = list.front()->get<MCNKstruct_s>();
  const MCNKstruct_s *mcnk_back = list.back()->get<MCNKstruct_s>();
  const glm::vec3 &pos_front = mcnk_front->position;
  const glm::vec3 &pos_back = mcnk_back->position;
  _bounding_box.reset(-pos_front.y, pos_front.z, -pos_front.x);
  _bounding_box.addInternalPoint(-pos_back.y, pos_back.z, -pos_back.x);
}

void ADTMesh_c::getTextureList(ChunkInfo_s* mtex_info, FilenameList_t* list)
{
  Buffer_t::const_iterator begin=mtex_info->buffer.begin();
  for (Buffer_t::const_iterator iter=begin; iter!=mtex_info->buffer.end(); ++iter) {
    if ((*iter)==0) {
      list->push_back(std::string(begin, iter));
      begin=iter+1;
    }
  }
}

void ADTMesh_c::loadTexture(const std::string& filename)
{
  ITexture *found = _driver->findTexture(filename.c_str());
  if (found!=0) {
    _texture_vector.push_back(found);
    return;
  }

  Buffer_t fb;
  _mpq_util->readFile(filename, &fb);

  BLP2Header_s *header = reinterpret_cast<BLP2Header_s*>(fb.data());
  uint32_t offset = header->offsets[0];
  uint32_t length = header->lengths[0];
  uint32_t width = header->width;
  uint32_t height = header->height;

  uint8_t *pixels = new uint8_t[width*height*4];

  switch (header->encoding) {
    case BLP_ENC_UNCOMPRESSED:
      _texture_vector.push_back(0);
      std::cout << "uncompressed" << std::endl;
      break;

    case BLP_ENC_DX_COMPRESSION: {
      Buffer_t blocks(fb.begin()+offset, fb.begin()+offset+length);
      switch (header->alpha_encoding) {
        case BLP_AE_DXT1:
          squish::DecompressImage(pixels, width, height, blocks.data(), squish::kDxt1);
          break;

        case BLP_AE_DXT23:
          squish::DecompressImage(pixels, width, height, blocks.data(), squish::kDxt3);
          break;

        case BLP_AE_DXT45:
          squish::DecompressImage(pixels, width, height, blocks.data(), squish::kDxt5);
          break;
      }
      IImage *image = _driver->createImageFromData(ECF_A8R8G8B8, dimension2du(width, height), pixels, true, true);
      ITexture *texture = _driver->addTexture(filename.c_str(), image, 0);
      _texture_vector.push_back(texture);
      break; }
  }

  /*cimg_library::CImg<uint32_t> image((uint32_t*)pixels, width, height);
  image.save_png("tmp.png", 4);*/
}

void ADTMesh_c::decompressAlphaMap(uint8_t* in, ARGB8_s* out, int index)
{
  uint32_t off_in = 0; //offset IN buffer
  uint32_t off_out = 0; //offset OUT buffer

  while(off_out<4096)
  {
    // fill or copy mode
    bool is_fill_mode = in[off_in] & 0x80;
    uint32_t n = in[off_in++] & 0x7F;

    for (uint32_t k=0; k<n; k++) {
      if (off_out==4096) { break; }
      out[off_out++].c[index] = in[off_in];
      if (!is_fill_mode) { off_in++; }
    }

    if (is_fill_mode) { off_in++; }
  }
}
