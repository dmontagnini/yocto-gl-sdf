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

#include "func_sdf.h"
#include "math_sdf.h"

#define PHI (sqrtf(5.0f)*0.5f + 0.5f)

float fSphere(vec3f p, float r) {
    return length(p) - r;
}

// Plane with normal n (n is normalized) at some distance from the origin
float fPlane(vec3f p, vec3f n, float distanceFromOrigin) {
    return dot(p, n) + distanceFromOrigin;
}

// Cheap Box: distance to corners is overestimated
float fBoxCheap(vec3f p, vec3f b) { //cheap box
    return vmax(vabs(p) - b);
}

// Box: correct distance to corners
float fBox(vec3f p, vec3f b) {
    vec3f d = vabs(p) - b;
    return length(vmax(d, vec3f(0))) + vmax(vmin(d, vec3f(0)));
}

// Same as above, but in two dimensions (an endless box)
float fBox2Cheap(vec2f p, vec2f b) {
    return vmax(vabs(p)-b);
}

float fBox2(vec2f p, vec2f b) {
    vec2f d = vabs(p) - b;
    return length(vmax(d, vec2f(0))) + vmax(vmin(d, vec2f(0)));
}


// Endless "corner"
float fCorner (vec2f p) {
    return length(vmax(p, vec2f(0))) + vmax(vmin(p, vec2f(0)));
}

// Blobby ball object. You've probably seen it somewhere. This is not a correct distance bound, beware.
float fBlob(vec3f p) {
    p = vabs(p);
    if (p.x < max(p.y, p.z)) p = vec3f{p.y,p.z,p.x};
//    if (p.x < max(p.y, p.z)) p = p.yzx;
    float b = max(max(max(
            dot(p, normalize(vec3f(1, 1, 1))),
            dot(vec2f{p.x,p.z}, normalize(vec2f(PHI+1, 1)))),
                      dot(vec2f{p.y,p.x}, normalize(vec2f(1, PHI)))),
                  dot(vec2f{p.x,p.z}, normalize(vec2f(1, PHI))));
    float l = length(p);
    return l - 1.5f - 0.2f * (1.5f / 2.0f)* cos(min(sqrt(1.01f - b / l)*(pif / 0.25f), pif));
}

// Cylinder standing upright on the xz plane
float fCylinder(vec3f p, float r, float height) {
    float d = length(vec2f{p.x,p.z}) - r;
    d = max(d, abs(p.y) - height);
    return d;
}

// Capsule: A Cylinder with round caps on both sides
float fCapsule(vec3f p, float r, float c) {
    return mix(length(vec2f{p.x,p.z}) - r, length(vec3f(p.x, abs(p.y) - c, p.z)) - r, step(c, abs(p.y)));
}

// Distance to line segment between <a> and <b>, used for fCapsule() version 2below
float fLineSegment(vec3f p, vec3f a, vec3f b) {
    vec3f ab = b - a;
    float t = saturate(dot(p - a, ab) / dot(ab, ab));
    return length((ab*t + a) - p);
}

// Capsule version 2: between two end points <a> and <b> with radius r
float fCapsule(vec3f p, vec3f a, vec3f b, float r) {
    return fLineSegment(p, a, b) - r;
}

// Torus in the XZ-plane
float fTorus(vec3f p, float smallRadius, float largeRadius) {
    return length(vec2f(length(vec2f{p.x,p.z}) - largeRadius, p.y)) - smallRadius;
}

// A circle line. Can also be used to make a torus by subtracting the smaller radius of the torus.
float fCircle(vec3f p, float r) {
    float l = length(vec2f{p.x,p.z}) - r;
    return length(vec2f(p.y, l));
}

// A circular disc with no thickness (i.e. a cylinder with no height).
// Subtract some value to make a flat disc with rounded edge.
float fDisc(vec3f p, float r) {
    float l = length(vec2f{p.x,p.z}) - r;
    return l < 0 ? abs(p.y) : length(vec2f(p.y, l));
}

// Hexagonal prism, circumcircle variant
float fHexagonCircumcircle(vec3f p, vec2f h) {
    vec3f q = vabs(p);
    return max(q.y - h.y, max(q.x*sqrt(3.0f)*0.5f + q.z*0.5f, q.z) - h.x);
    //this is mathematically equivalent to this line, but less efficient:
    //return max(q.y - h.y, max(dot(vec2(cos(PI/3), sin(PI/3)), q.zx), q.z) - h.x);
}

// Hexagonal prism, incircle variant
float fHexagonIncircle(vec3f p, vec2f h) {
    return fHexagonCircumcircle(p, vec2f(h.x*sqrt(3.0f)*0.5f, h.y));
}

// Cone with correct distances to tip and base circle. Y is up, 0 is in the middle of the base.
float fCone(vec3f p, float radius, float height) {
    vec2f q = vec2f(length(vec2f{p.x,p.z}), p.y);
    vec2f tip = q - vec2f(0, height);
    vec2f mantleDir = normalize(vec2f(height, radius));
    float mantle = dot(tip, mantleDir);
    float d = max(mantle, -q.y);
    float projected = dot(tip, vec2f(mantleDir.y, -mantleDir.x));

    // distance to tip
    if ((q.y > height) && (projected < 0)) {
        d = max(d, length(tip));
    }

    // distance to base ring
    if ((q.x > radius) && (projected > length(vec2f(height, radius)))) {
        d = max(d, length(q - vec2f(radius, 0)));
    }
    return d;
}


float fIntersect(float d1, float d2) {
	return max(d1, d2);
}

float fUnion(float d1, float d2) {
	return min(d1, d2);
}

float fDifference(float d1, float d2) {
	return max(d1, -d2);
}


// The "Chamfer" flavour makes a 45-degree chamfered edge (the diagonal of a square of size <r>):
float fOpUnionChamfer(float a, float b, float r) {
    return min(min(a, b), (a - r + b)*sqrtf(0.5f));
}

// Intersection has to deal with what is normally the inside of the resulting object
// when using union, which we normally don't care about too much. Thus, intersection
// implementations sometimes differ from union implementations.
float fOpIntersectionChamfer(float a, float b, float r) {
    return max(max(a, b), (a + r + b)*sqrtf(0.5f));
}

// Difference can be built from Intersection or Union:
float fOpDifferenceChamfer (float a, float b, float r) {
    return fOpIntersectionChamfer(a, -b, r);
}

// The "Round" variant uses a quarter-circle to join the two objects smoothly:
float fOpUnionRound(float a, float b, float r) {
    vec2f u = vmax(vec2f(r - a,r - b), vec2f(0.0f));
    return max(r, min (a, b)) - length(u);
}

float fOpIntersectionRound(float a, float b, float r) {
    vec2f u = vmax(vec2f(r + a,r + b), vec2f(0));
    return min(-r, max (a, b)) + length(u);
}

float fOpDifferenceRound (float a, float b, float r) {
    return fOpIntersectionRound(a, -b, r);
}


// The "Columns" flavour makes n-1 circular columns at a 45 degree angle:
float fOpUnionColumns(float a, float b, float r, float n) {
    if ((a < r) && (b < r)) {
        vec2f p = vec2f(a, b);
        float columnradius = r*sqrtf(2.0f)/((n-1.0f)*2.0f+sqrtf(2.0f));
        p = pR45(p);
        p.x -= sqrt(2)/2*r;
        p.x += columnradius*sqrt(2);
        if (mod(n,2) == 1) {
            p.y += columnradius;
        }
        // At this point, we have turned 45 degrees and moved at a point on the
        // diagonal that we want to place the columns on.
        // Now, repeat the domain along this direction and place a circle.
        pMod1(p.y, columnradius*2);
        float result = length(p) - columnradius;
        result = min(result, p.x);
        result = min(result, a);
        return min(result, b);
    } else {
        return min(a, b);
    }
}

float fOpDifferenceColumns(float a, float b, float r, float n) {
    a = -a;
    float m = min(a, b);
    //avoid the expensive computation where not needed (produces discontinuity though)
    if ((a < r) && (b < r)) {
        vec2f p = vec2f(a, b);
        float columnradius = r*sqrtf(2.0f)/n/2.0f;
        columnradius = r*sqrtf(2.0f)/((n-1)*2+sqrtf(2.0f));

        p = pR45(p);
        p.y += columnradius;
        p.x -= sqrt(2)/2*r;
        p.x += -columnradius*sqrt(2)/2;

        if (mod(n,2) == 1) {
            p.y += columnradius;
        }
        pMod1(p.y,columnradius*2);

        float result = -length(p) + columnradius;
        result = max(result, p.x);
        result = min(result, a);
        return -min(result, b);
    } else {
        return -m;
    }
}

float fOpIntersectionColumns(float a, float b, float r, float n) {
    return fOpDifferenceColumns(a,-b,r, n);
}

// The "Stairs" flavour produces n-1 steps of a staircase:
// much less stupid version by paniq
float fOpUnionStairs(float a, float b, float r, float n) {
    float s = r/n;
    float u = b-r;
    return min(min(a,b), 0.5f * (u + a + abs ((mod(u - a + s, 2.0f * s)) - s)));
}

// We can just call Union since stairs are symmetric.
float fOpIntersectionStairs(float a, float b, float r, float n) {
    return -fOpUnionStairs(-a, -b, r, n);
}

float fOpDifferenceStairs(float a, float b, float r, float n) {
    return -fOpUnionStairs(-a, b, r, n);
}


// Similar to fOpUnionRound, but more lipschitz-y at acute angles
// (and less so at 90 degrees). Useful when fudging around too much
// by MediaMolecule, from Alex Evans' siggraph slides
float fOpUnionSoft(float a, float b, float r) {
    float e = max(r - abs(a - b), 0.0f);
    return min(a, b) - e*e*0.25f/r;
}


// produces a cylindical pipe that runs along the intersection.
// No objects remain, only the pipe. This is not a boolean operator.
float fOpPipe(float a, float b, float r) {
    return length(vec2f(a, b)) - r;
}

// first object gets a v-shaped engraving where it intersect the second
float fOpEngrave(float a, float b, float r) {
    return max(a, (a + r - abs(b))*sqrtf(0.5f));
}

// first object gets a capenter-style groove cut out
float fOpGroove(float a, float b, float ra, float rb) {
    return max(a, min(a + ra, rb - abs(b)));
}

// first object gets a capenter-style tongue attached
float fOpTongue(float a, float b, float ra, float rb) {
    return min(a, max(a - ra, abs(b) - rb));
}