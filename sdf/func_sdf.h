//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"
#include "math_sdf.h"

using namespace ygl;

#ifndef YOCTO_GL_DISTANCE_FUNCTIONS_H
#define YOCTO_GL_DISTANCE_FUNCTIONS_H

float sdSphere(vec3f pos, float r);

float udBox(vec3f p, vec3f b);

float udRoundBox(vec3f p, vec3f b, float r);

float sdBox(vec3f p, vec3f b);

float sdTorus(vec3f p, vec2f t);

float sdCylinder(vec3f p, vec3f c);

float sdCone(vec3f p, vec2f c);

float sdPlane(vec3f p, vec4f n);

float sdHexPrism(vec3f p, vec2f h);

float sdTriPrism(vec3f p, vec2f h) ;

float sdCapsule(vec3f p, vec3f a, vec3f b, float r);

float sdCappedCylinder(vec3f p, vec2f h);

float sdCappedCone(vec3f p, vec3f c);

float sdEllipsoid(vec3f p, vec3f r);

#endif //YOCTO_GL_DISTANCE_FUNCTIONS_H
