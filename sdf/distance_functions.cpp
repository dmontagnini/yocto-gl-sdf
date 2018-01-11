//
// Created by Dario Montagnini on 11/01/18.
//

#include "distance_functions.h"

float sphereSDF(vec3f pos, float r){
    return length(pos) - r;
}

float boxSDF(vec3f pos, vec3f size){
    return length(max(abs(pos)-size,{0.0f,0.0f,0.0f}));
}

float sdPlane( vec3f p, vec4f n )
{
    // n must be normalized
    return dot(p,vec3f{n.x, n.y, n.z}) + n.w;
}