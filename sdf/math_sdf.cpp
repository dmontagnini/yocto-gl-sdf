//
// Created by Dario Montagnini on 11/01/18.
//

#include "math_sdf.h"

vec3f abs(vec3f v){
    return {abs(v.x),abs(v.y),abs(v.z)};
}

vec3f max(vec3f v, vec3f u){
    return {max(v.x,u.x),max(v.y,u.y),max(v.z,u.z)};
}

float intersectSDF(float d1, float d2){
    return max(d1,d2);
}

float unionSDF(float d1, float d2){
    return min(d1,d2);
}

float differenceSDF(float d1, float d2){
    return max(d1,-d2);
}

