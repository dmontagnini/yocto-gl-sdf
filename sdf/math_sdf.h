//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"

using namespace ygl;

#ifndef YOCTO_GL_MATH_SDF_H
#define YOCTO_GL_MATH_SDF_H

vec3f abs(vec3f v);

vec3f max(vec3f v, vec3f u);

float intersectSDF(float d1, float d2);

float unionSDF(float d1, float d2);

float differenceSDF(float d1, float d2);

mat4f rotateY(float theta);

#endif //YOCTO_GL_MATH_SDF_H
