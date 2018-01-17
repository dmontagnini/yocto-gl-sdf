//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"
#include "func_sdf.h"

using namespace ygl;

#ifndef YOCTO_GL_SCENE_SDF_H
#define YOCTO_GL_SCENE_SDF_H

struct light_sdf {
    vec3f pos;
    vec3f ke;
};

camera make_camera();

std::vector<material*> make_materials();

std::vector<light_sdf*> make_lights();

vec2f texture_mapping(vec3f pos, float id);

vec2f sceneSDF(vec3f pos);

#endif //YOCTO_GL_SCENE_SDF_H
