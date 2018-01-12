//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"
#include "distance_functions.h"

using namespace ygl;

#ifndef YOCTO_GL_SCENE_SDF_H
#define YOCTO_GL_SCENE_SDF_H

struct light_sdf {
    vec3f pos;
    vec3f ke;
};

camera set_camera();

vec3f make_materials(float id);

vec2f texture_mapping(vec3f pos, float id);

vec3f set_environment();

std::vector<light_sdf*> set_lights();

vec2f sceneSDF(vec3f pos);

#endif //YOCTO_GL_SCENE_SDF_H
