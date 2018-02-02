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

void load_textures();

camera make_camera();

std::vector<light_sdf*> make_lights();

vec2f fScene(vec3f pos);

material materials(float id, vec3f pos);

#endif //YOCTO_GL_SCENE_SDF_H
