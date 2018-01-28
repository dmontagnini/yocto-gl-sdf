//
// Created by Dario Montagnini on 11/01/18.
//

#include "math_sdf.h"

vec2f abs(vec2f v) {
	return { abs(v.x),abs(v.y) };
}

vec3f abs(vec3f v) {
	return { abs(v.x),abs(v.y),abs(v.z) };
}

vec2f max(vec2f v, vec2f u) {
	return { max(v.x,u.x),max(v.y,u.y) };
}

vec3f max(vec3f v, vec3f u) {
	return { max(v.x,u.x),max(v.y,u.y),max(v.z,u.z) };
}

vec3f min(vec3f v, vec3f u) {
    return { min(v.x,u.x),min(v.y,u.y),min(v.z,u.z) };
}

vec2f min(vec2f v, vec2f u) {
    return { min(v.x,u.x),min(v.y,u.y) };
}

float intersectSDF(float d1, float d2) {
	return max(d1, d2);
}

float unionSDF(float d1, float d2) {
	return min(d1, d2);
}

float differenceSDF(float d1, float d2) {
	return max(d1, -d2);
}

float smin(float d1, float d2, float k) {
	float h = clamp(0.5 + 0.5*(d2 - d1) / k, 0.0f, 1.0f);
	return mix(d2, d1, h) - k*h*(1.0f - h);
}

float smax(float d1, float d2, float k) {
	float h = clamp(0.5 + 0.5*(d2 - d1) / k, 0.0f, 1.0f);
	return mix(d1, d2, h) - k*h*(1.0f - h);
}

float sign(float x)
{
	if (x < 0.0f) return -1.0f;
	if (x > 0.0f) return  1.0f;
	return 0.0f;
}

float mix(float a, float b, float x)
{
	return a + (b - a)*x;
}

mat3f rotateX(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return mat3f(
		vec3f(1, 0, 0),
		vec3f(0, c, -s),
		vec3f(0, s, c)
	);
}

mat3f rotateY(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return mat3f(
		vec3f(c, 0, s),
		vec3f(0, 1, 0),
		vec3f(-s, 0, c)
	);
}

mat3f rotateZ(float theta) {
	float c = cos(theta);
	float s = sin(theta);

	return mat3f(
		vec3f(c, -s, 0),
		vec3f(s, c, 0),
		vec3f(0, 0, 1)
	);
}


float mymod(float x, float y){
    auto res = fmodf(x,y);
    if (res < 0) res += y;
    return res;
}