//
// Created by Dario Montagnini on 11/01/18.
//

#include "../yocto/yocto_gl.h"
#include "math_sdf.h"

using namespace ygl;

#ifndef YOCTO_GL_DISTANCE_FUNCTIONS_H
#define YOCTO_GL_DISTANCE_FUNCTIONS_H

float fSphere(vec3f p, float r);

float fPlane(vec3f p, vec3f n, float distanceFromOrigin);

float fBoxCheap(vec3f p, vec3f b);

float fBox(vec3f p, vec3f b);

float fBox2Cheap(vec2f p, vec2f b);

float fBox2(vec2f p, vec2f b);

float fCorner (vec2f p);

float fBlob(vec3f p);

float fCylinder(vec3f p, float r, float height);

float fCapsule(vec3f p, float r, float c);

float fLineSegment(vec3f p, vec3f a, vec3f b);

float fCapsule(vec3f p, vec3f a, vec3f b, float r);

float fTorus(vec3f p, float smallRadius, float largeRadius);

float fCircle(vec3f p, float r);

float fDisc(vec3f p, float r);

float fHexagonCircumcircle(vec3f p, vec2f h);

float fHexagonIncircle(vec3f p, vec2f h);

float fCone(vec3f p, float radius, float height);

float fIntersect(float d1, float d2);

float fUnion(float d1, float d2);

float fDifference(float d1, float d2);

float fOpUnionChamfer(float a, float b, float r);

float fOpIntersectionChamfer(float a, float b, float r);

float fOpDifferenceChamfer (float a, float b, float r);

float fOpUnionRound(float a, float b, float r);

float fOpIntersectionRound(float a, float b, float r);

float fOpDifferenceRound (float a, float b, float r);

float fOpUnionColumns(float a, float b, float r, float n);

float fOpDifferenceColumns(float a, float b, float r, float n);

float fOpIntersectionColumns(float a, float b, float r, float n);

float fOpUnionStairs(float a, float b, float r, float n);

float fOpIntersectionStairs(float a, float b, float r, float n);

float fOpDifferenceStairs(float a, float b, float r, float n);

float fOpUnionSoft(float a, float b, float r);

float fOpPipe(float a, float b, float r);

float fOpEngrave(float a, float b, float r);

float fOpGroove(float a, float b, float ra, float rb);

float fOpTongue(float a, float b, float ra, float rb);


#endif //YOCTO_GL_DISTANCE_FUNCTIONS_H
