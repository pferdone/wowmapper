/*
 * shader_callback.hpp
 *
 *  Created on: Nov 11, 2012
 *      Author: pferdone
 */

#ifndef SHADER_CALLBACK_HPP_
#define SHADER_CALLBACK_HPP_

#include <IShaderConstantSetCallBack.h>
#include <IMaterialRendererServices.h>

class ShaderCallback_c : public irr::video::IShaderConstantSetCallBack
{
  public:
    virtual void OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 user_data)
    {
      irr::video::IVideoDriver* driver = services->getVideoDriver();

      //
      irr::core::matrix4 world_view_proj_matrix;
      world_view_proj_matrix = driver->getTransform(irr::video::ETS_PROJECTION);
      world_view_proj_matrix *= driver->getTransform(irr::video::ETS_VIEW);
      world_view_proj_matrix *= driver->getTransform(irr::video::ETS_WORLD);

      services->setVertexShaderConstant("mWorldViewProj", world_view_proj_matrix.pointer(), 16);

      irr::s32 texture_layer_id = 0;
      services->setPixelShaderConstant("mTexture0", &texture_layer_id, 1);
      texture_layer_id = 1;
      services->setPixelShaderConstant("mTexture1", &texture_layer_id, 1);
      texture_layer_id = 2;
      services->setPixelShaderConstant("mTexture2", &texture_layer_id, 1);
      texture_layer_id = 3;
      services->setPixelShaderConstant("mTexture3", &texture_layer_id, 1);
      texture_layer_id = 4;
      services->setPixelShaderConstant("mTexture4", &texture_layer_id, 1);
    }
};


#endif /* SHADER_CALLBACK_HPP_ */
