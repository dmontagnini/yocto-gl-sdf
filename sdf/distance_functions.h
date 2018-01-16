//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"
#include "math_sdf.h"

using namespace ygl;

#ifndef YOCTO_GL_DISTANCE_FUNCTIONS_H
#define YOCTO_GL_DISTANCE_FUNCTIONS_H

float sphereSDF(vec3f pos, float r);

float boxSDF(vec3f pos, vec3f size);

float sdPlane( vec3f p, vec4f n );

float sdCappedCylinder( vec3f p, vec2f h );

#endif //YOCTO_GL_DISTANCE_FUNCTIONS_H
