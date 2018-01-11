//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"

#define MAT_CUBE 1.0
#define MAT_SPHERE 2.0
#define MAT_PLANE 3.0

camera set_camera() {

    auto cam = camera();

    cam.frame.o = {0.0f,5.0f,10.0f};
    cam.frame.x = {1.0f,0.0f,0.0f};
    cam.frame.y = {0.0f, 0.957826f, -0.287348f};
    cam.frame.z = {0.0f, 0.287348f, 0.957826f};
    cam.yfov = 0.261799f;

    return cam;
}

vec3f make_materials(float id) {
    if(id == 1.0f) return vec3f{1.0f, 0.0f, 0.0f};
    else if(id == 2.0f) return vec3f{0.0f, 1.0f, 0.0f};
    else return vec3f{0.0f, 0.0f, 1.0f};
}

vec3f set_environment() {
    return {0.5f, 0.5f, 0.5f};
}

std::vector<light_sdf*> set_lights() {
    auto lights = std::vector<light_sdf*>();
    auto l = light_sdf{{0.0f,5.0f,10.0f}, {200, 200, 200}};
    lights.push_back(&l);

    return lights;
}

vec2f sceneSDF(vec3f pos) {
    float d_plane = sdPlane(pos,normalize(vec4f{0.0f,1.0f,0.0f,0.0f}));
    float d_sphere = sphereSDF(pos + vec3f{1.0f,-0.5f,2.0f}, 0.5f);
    float d_cube = boxSDF(pos + vec3f{-1.0f,-0.5f,2.0f},{0.5f,0.5f,0.50f});
    if(d_plane < d_cube)
        if(d_plane < d_sphere) return {d_plane, MAT_PLANE};
        else return {d_sphere, MAT_SPHERE};
    else
        if(d_cube < d_sphere) return {d_cube, MAT_CUBE};
        else return {d_sphere,MAT_SPHERE};
}
