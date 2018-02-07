//
// Created by Mattei Francesca Romana, Montagnini Dario and Riso Marzia on 11/01/18.
//

#include "../yocto/yocto_gl.h"


using namespace ygl;

#ifndef YOCTO_GL_MATH_SDF_H
#define YOCTO_GL_MATH_SDF_H

float step(float edge, float x);

float saturate(float x);

float sgn(float x);

vec2f vsgn(vec2f v);

float vmax(vec2f v);

float vmax(vec3f v);

vec2f vmax(vec2f v,vec2f u);

vec3f vmax(vec3f v,vec3f u);

float vmin(vec2f v);

float vmin(vec3f v);

vec2f vmin(vec2f v,vec2f u);

vec3f vmin(vec3f v,vec3f u);

vec2f vabs(vec2f v);

vec3f vabs(vec3f v);

float mix(float a, float b, float x);

vec2f vmix(vec2f a, vec2f b, vec2f x);

vec3f vmix(vec3f a, vec3f b, vec3f x);

float mod(float x, float y);

vec2f vmod(vec2f x,vec2f y);

vec3f vmod(vec3f x,vec3f y);

vec2f vfloor(vec2f v);

vec3f vfloor(vec3f v);

vec2f pR(vec2f p, float a);

vec2f pR45(vec2f p);

float pMod1(float p, float size);

float pModMirror1(float p, float size);

float pModSingle1(float p, float size);

float pModInterval1(float p, float size, float start, float stop);

vec2f pModPolar(vec2f p, float repetitions);

vec2f pMod2(vec2f p, vec2f size);

vec2f pModMirror2(vec2f p, vec2f size);

vec2f pModGrid2(vec2f p, vec2f size);

vec3f pMod3(vec3f p, vec3f size);

float pMirror (float p, float dist);

vec2f pMirrorOctant (vec2f p, vec2f dist);

vec3f pReflect(vec3f p, vec3f planeNormal, float offset);

mat3f rotateX(float theta);

mat3f rotateY(float theta);

mat3f rotateZ(float theta);

#endif //YOCTO_GL_MATH_SDF_H
