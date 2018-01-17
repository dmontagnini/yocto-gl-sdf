//
// Created by Dario Montagnini on 11/01/18.
//

#include "math_sdf.h"

vec2f abs(vec2f v){
    return {abs(v.x),abs(v.y)};
}

vec3f abs(vec3f v){
    return {abs(v.x),abs(v.y),abs(v.z)};
}

vec2f max(vec2f v, vec2f u){
    return {max(v.x,u.x),max(v.y,u.y)};
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

float sign( float x )
{
    if( x<0.0f ) return -1.0f;
    if( x>0.0f ) return  1.0f;
    return 0.0f;
}

mat4f rotateY(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    return mat4f(
            vec4f(c, 0, s, 0),
            vec4f(0, 1, 0, 0),
            vec4f(-s, 0, c, 0),
            vec4f(0, 0, 0, 1)
    );
}