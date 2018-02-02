//
//                           HG_SDF
//
//     GLSL LIBRARY FOR BUILDING SIGNED DISTANCE BOUNDS
//
//     version 2016-01-10
//
//     Check http://mercury.sexy/hg_sdf for updates
//     and usage examples. Send feedback to spheretracing@mercury.sexy.
//
//     Brought to you by MERCURY http://mercury.sexy
//
//
//
// Released as Creative Commons Attribution-NonCommercial (CC BY-NC)
//

//
// Created by Dario Montagnini on 11/01/18.
//

#include "math_sdf.h"

float step(float edge, float x) {
    return x < edge ? 0.0f : 1.0f;
}

float saturate(float x) {
    return clamp(x, 0.0f, 1.0f);
}

// Sign function that doesn't return 0
float sgn(float x) {
    return (x<0.0f)?-1.0f:1.0f;
}

vec2f vsgn(vec2f v) {
    return vec2f((v.x<0.0f)?-1.0f:1.0f, (v.y<0.0f)?-1.0f:1.0f);
}

// Maximum/minumum elements of a vector
float vmax(vec2f v) {
    return max(v.x, v.y);
}

float vmax(vec3f v) {
    return max(max(v.x, v.y), v.z);
}

vec2f vmax(vec2f v,vec2f u) {
    return {max(v.x, u.x),max(v.y, u.y)};
}

vec3f vmax(vec3f v,vec3f u) {
    return {max(v.x, u.x),max(v.y, u.y), max(v.z,u.z)};
}

float vmin(vec2f v) {
    return min(v.x, v.y);
}

float vmin(vec3f v) {
    return min(min(v.x, v.y), v.z);
}

vec2f vmin(vec2f v,vec2f u) {
    return {min(v.x, u.x),min(v.y, u.y)};
}

vec3f vmin(vec3f v,vec3f u) {
    return {min(v.x, u.x),min(v.y, u.y), min(v.z,u.z)};
}

vec2f vabs(vec2f v) {
	return { abs(v.x),abs(v.y) };
}

vec3f vabs(vec3f v) {
	return { abs(v.x),abs(v.y),abs(v.z) };
}


float mix(float a, float b, float x) {
    return a + (b - a)*x;
}

vec2f vmix(vec2f a, vec2f b, vec2f x) {
    return {mix(a.x,b.x,x.x),mix(a.y,b.y,x.y)};
}

vec3f vmix(vec3f a, vec3f b, vec3f x) {
    return {mix(a.x,b.x,x.x),mix(a.y,b.y,x.y),mix(a.z,b.z,x.z)};
}


float mod(float x, float y){
    auto res = fmodf(x,y);
    if (res < 0) res += y;
    return res;
}

vec2f vmod(vec2f x,vec2f y){
    auto res = vec2f{fmodf(x.x,y.x),fmodf(x.y,y.y)};
    if (res.x < 0) res.x += y.x;
    if (res.y < 0) res.y += y.y;
    return res;
}

vec3f vmod(vec3f x,vec3f y){
    auto res = vec3f{fmodf(x.x,y.x),fmodf(x.y,y.y), fmodf(x.z,y.z)};
    if (res.x < 0) res.x += y.x;
    if (res.y < 0) res.y += y.y;
    if (res.z < 0) res.z += y.z;
    return res;
}

vec2f vfloor(vec2f v){
    return {floorf(v.x),floorf(v.y)};
}

vec3f vfloor(vec3f v){
    return {floorf(v.x),floorf(v.y),floorf(v.z)};
}

// Rotate around a coordinate axis (i.e. in a plane perpendicular to that axis) by angle <a>.
// Read like this: R(p.xz, a) rotates "x towards z".
// This is fast if <a> is a compile-time constant and slower (but still practical) if not.
vec2f pR(vec2f p, float a) {
    p = cos(a)*p + sin(a)*vec2f(p.y, -p.x);
    return p;
}

// Shortcut for 45-degrees rotation
vec2f pR45(vec2f p) {
    p = (p + vec2f(p.y, -p.x))*sqrt(0.5);
    return p;
}

// Repeat space along one axis. Use like this to repeat along the x axis:
// <float cell = pMod1(p.x,5);> - using the return value is optional.
float pMod1(float p, float size) {
    float halfsize = size*0.5f;
//    float c = floor((p + halfsize)/size);
    p = mod(p + halfsize, size) - halfsize;
//    return c;
    return p;
}

// Same, but mirror every second cell so they match at the boundaries
float pModMirror1(float p, float size) {
    float halfsize = size*0.5f;
    float c = floor((p + halfsize)/size);
    p = mod(p + halfsize,size) - halfsize;
    p *= mod(c, 2.0)*2 - 1;
//    return c;
    return p;
}

// Repeat the domain only in positive direction. Everything in the negative half-space is unchanged.
float pModSingle1(float p, float size) {
    float halfsize = size*0.5f;
//    float c = floor((p + halfsize)/size);
    if (p >= 0)
        p = mod(p + halfsize, size) - halfsize;
//    return c;
    return p;
}

// Repeat only a few times: from indices <start> to <stop> (similar to above, but more flexible)
float pModInterval1(float p, float size, float start, float stop) {
    float halfsize = size*0.5f;
    float c = floor((p + halfsize)/size);
    p = mod(p+halfsize, size) - halfsize;
    if (c > stop) { //yes, this might not be the best thing numerically.
        p += size*(c - stop);
        c = stop;
    }
    if (c <start) {
        p += size*(c - start);
        c = start;
    }
//    return c;
    return p;
}


// Repeat around the origin by a fixed angle.
// For easier use, num of repetitions is use to specify the angle.
vec2f pModPolar(vec2f p, float repetitions) {
    float angle = 2*pif/repetitions;
    float a = atan2(p.y, p.x) + angle/2.0f;
    float r = length(p);
    float c = floor(a/angle);
    a = mod(a,angle) - angle/2.0f;
    p = vec2f(cos(a), sin(a))*r;
    // For an odd number of repetitions, fix cell index of the cell in -x direction
    // (cell index would be e.g. -5 and 5 in the two halves of the cell):
    if (abs(c) >= (repetitions/2)) c = abs(c);
//    return c;
    return p;
}

// Repeat in two dimensions
vec2f pMod2(vec2f p, vec2f size) {
//    vec2f c = vfloor((p + size*0.5f)/size);
    p = vmod(p + size*0.5f,size) - size*0.5;
//    return c;
    return p;
}

// Same, but mirror every second cell so all boundaries match
vec2f pModMirror2(vec2f p, vec2f size) {
    vec2f halfsize = size*0.5f;
    vec2f c = vfloor((p + halfsize)/size);
    p = vmod(p + halfsize, size) - halfsize;
    p *= vmod(c,vec2f(2.0f))*2.0f - vec2f(1.0f);
//    return c;
    return p;
}

// Same, but mirror every second cell at the diagonal as well
vec2f pModGrid2(vec2f p, vec2f size) {
    vec2f c = vfloor((p + size*0.5)/size);
    p = vmod(p + size*0.5f, size) - size*0.5;
    p *= vmod(c,vec2f(2.0))*2 - vec2f(1.0);
    p -= size/2.0f;
    if (p.x > p.y) p = vec2f(p.y,p.x);
    return p;
//    return vfloor(c/2);
}

// Repeat in three dimensions
vec3f pMod3(vec3f p, vec3f size) {
//    vec3f c = vfloor((p + size*0.5f)/size);
    p = vmod(p + size*0.5f, size) - size*0.5f;
//    return c;
    return p;
}

// Mirror at an axis-aligned plane which is at a specified distance <dist> from the origin.
float pMirror (float p, float dist) {
    float s = sgn(p);
    p = abs(p)-dist;
//    return s;
    return p;
}

// Mirror in both dimensions and at the diagonal, yielding one eighth of the space.
// translate by dist before mirroring.
vec2f pMirrorOctant (vec2f p, vec2f dist) {
    vec2f s = vsgn(p);
    pMirror(p.x, dist.x);
    pMirror(p.y, dist.y);
    if (p.y > p.x)
        p = vec2f{p.y,p.x};
//    return s;
    return p;
}

// Reflect space at a plane
vec3f pReflect(vec3f p, vec3f planeNormal, float offset) {
    float t = dot(p, planeNormal)+offset;
    if (t < 0) {
        p = p - (2*t)*planeNormal;
    }
//    return sgn(t);
    return p;
}



//float smin(float d1, float d2, float k) {
//	float h = clamp(0.5 + 0.5*(d2 - d1) / k, 0.0f, 1.0f);
//	return mix(d2, d1, h) - k*h*(1.0f - h);
//}
//
//float smax(float d1, float d2, float k) {
//	float h = clamp(0.5 + 0.5*(d2 - d1) / k, 0.0f, 1.0f);
//	return mix(d1, d2, h) - k*h*(1.0f - h);
//}


mat3f rotateX(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return {vec3f(1, 0, 0),
            vec3f(0, c, -s),
            vec3f(0, s, c)};
}

mat3f rotateY(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return {vec3f(c, 0, s),
            vec3f(0, 1, 0),
            vec3f(-s, 0, c)};
}

mat3f rotateZ(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return{vec3f(c, -s, 0),
           vec3f(s, c, 0),
           vec3f(0, 0, 1)};
}
