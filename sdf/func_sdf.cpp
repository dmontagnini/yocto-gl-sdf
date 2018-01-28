//
// Created by Dario Montagnini on 11/01/18.
//

#include "func_sdf.h"

// r = raggio
float sdSphere(vec3f pos, float r) {
    return length(pos) - r;
}

// b = (lunghezza, altezza, profondita')
float udBox(vec3f p, vec3f b) {
    return length(max(abs(p)-b,vec3f()));
}

// b = (lunghezza, altezza, profondita')
// r = raggio angolo arrotondato
float udRoundBox(vec3f p, vec3f b, float r) {
    return length(max(abs(p)-b,vec3f()))-r;
}

// b = (lunghezza, altezza, profondita')
// Ma che cambia con udBox ?
float sdBox(vec3f p, vec3f b) {
    vec3f d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,vec3f()));
}

// t = (raggio interno torus, spessore torus)
float sdTorus(vec3f p, vec2f t) {
    vec2f q = vec2f(length(vec2f{p.x,p.z})-t.x,p.y);
    return length(q)-t.y;
}

// c = (spostamento rispetto al centro, non cambia nulla , raggio cilindro)
float sdCylinder(vec3f p, vec3f c) {
    return length(vec2f{p.x,p.z}-vec2f{c.x,c.y})-c.z;
}

// c = (raggio cono, altezza cono)
float sdCone(vec3f p, vec2f c) {
    // c must be normalized
    float q = length(vec2f{p.x,p.y});
    return dot(c,vec2f(q,p.z));
}

float sdPlane(vec3f p, vec4f n) {
    // n must be normalized
    return dot(p,vec3f{n.x, n.y, n.z}) + n.w;
}

// h = (lato esagono, altezza prisma)
float sdHexPrism(vec3f p, vec2f h) {
    vec3f q = abs(p);
    return max(q.z-h.y,max((q.x*0.866025f+q.y*0.5f),q.y)-h.x);
}

// h = (lato triangolo, altezza prisma)
float sdTriPrism(vec3f p, vec2f h) {
    vec3f q = abs(p);
    return max(q.z-h.y,max(q.x*0.866025f+p.y*0.5f,-p.y)-h.x*0.5f);
}

// a = ???
// b = ???
// r = raggio estremita' arrotondate
float sdCapsule(vec3f p, vec3f a, vec3f b, float r) {
    vec3f pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}


// h = (raggio cilindro ,altezza cilindro)
float sdCappedCylinder(vec3f p, vec2f h) {
    vec2f d = abs(vec2f(length(vec2f{p.x,p.z}),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,vec2f{0.0f,0.0f}));
}


float sdCappedCone(vec3f p, vec3f c) {
    vec2f q = vec2f( length(vec2f(p.x,p.z)), p.y );
    vec2f v = vec2f( c.z*c.y/c.x, -c.z );
    vec2f w = v - q;
    vec2f vv = vec2f( dot(v,v), v.x*v.x );
    vec2f qv = vec2f( dot(v,w), v.x*w.x );
    vec2f d = max(qv,vec2f())*qv/vv;
    return sqrt( dot(w,w) - max(d.x,d.y) ) * sign(max(q.y*v.x-q.x*v.y,w.y));
//    return 1.0f;
}

float sdEllipsoid(vec3f pos, vec3f cen,vec3f rad )
{
    vec3f p = pos - cen;
    float d = length(p/rad) - 1.0f;
    return d * min(min(rad.x,rad.y),rad.z);
}

vec2f sdLine(vec3f pos,vec3f a,vec3f b )
{
    vec3f pa = pos - a;
    vec3f ba = b - a;

    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );

    return vec2f( length(pa-h*ba), h );
}

float sdCross(vec3f pos){
    float da = sdBox(pos,vec3f{MAXFLOAT,1.0f,1.0f});
    float db = sdBox(pos,vec3f{1.0f,MAXFLOAT,1.0f});
    float dc = sdBox(pos,vec3f{1.0f,1.0f,MAXFLOAT});
    return min(da,min(db,dc));
}

//// r = (lunghezza ellissoide,altezza ellissoide, profondita' ellissoide)
//float sdEllipsoid(vec3f p, vec3f r) {
//    return (length( p/r ) - 1.0f) * min(min(r.x,r.y),r.z);
//}