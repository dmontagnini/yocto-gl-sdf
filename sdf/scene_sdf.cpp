//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"

#define MAT_CUBE 1.0
#define MAT_SPHERE 2.0
#define MAT_PLANE 3.0

camera set_camera() {

    auto cam = camera();

    cam.frame.o = {0.0f,4.0f,10.0f};
    //cam.frame.o = {-1.0f, 4.0f, 10.0f};
    cam.frame.x = {1.0f,0.0f,0.0f};
    cam.frame.y = {0.0f, 0.957826f, -0.287348f};
    cam.frame.z = {0.0f, 0.287348f, 0.957826f};
    cam.yfov = 0.261799f;

    return cam;
}

vec3f make_materials(float id) {
    if(id == 1.0f) return vec3f{0.96f, 0.96f, 0.96f};
    else if(id == 2.0f) return vec3f{0.96f, 0.96f, 0.96f}; //return vec3f{0.192f, 0.48f, 0.192f};
    else return vec3f{0.2f, 0.2f, 0.2f};

}

vec2f texture_mapping(vec3f pos, float id) {
    vec2f uv = vec2f();
    if (id == 1.0) {
//        auto r  = 0.5f;
//        float phi = atan(pos.y/pos.x);
//        float teta = acos(fmod(pos.z/r,1.0f));
////        printf("%f %f\n",pos.z,teta);
//        uv.x = fmod(phi/(2*pif),1.0f);
//        uv.y = fmod(teta/pif,1.0f);
////        printf("%f %f\n",uv.x,uv.y);
        uv.x = 1.0f/32.0f;
        uv.y = 1.0f/32.0f;

    }
    else if (id == 2.0) {

//        auto r  = 0.5f;
//        float phi = atan(pos.y/pos.x);
//        float teta = acos(fmod(pos.z/r,1.0f));
//        printf("%f %f\n",pos.z,teta);
//        fflush(stdout);
//        uv.x = fmod(phi/(2*pif),1.0f);
//        uv.y = fmod(teta/pif,1.0f);
//           printf("%f %f\n",uv.x,uv.y);
        uv.x = 9.0f/32.0f;
        uv.y = 1.0f/32.0f;
    }
    else if (id == 3.0){
        uv.x = fmod(abs(pos.x),1.0f);
        uv.y = fmod(abs(pos.z),1.0f);
    }
    return uv;
}



vec3f set_environment() {
    return {0.1f, 0.1f, 0.1f};
}

std::vector<light_sdf*> set_lights() {
    auto lights = std::vector<light_sdf*>();
    auto l1 = light_sdf{{1.4f,8.0f,6.0f}, {100, 100, 100}};
    auto l2 = light_sdf{{-1.4f,8.0f,0.0f}, {100, 100, 100}};
    lights.push_back(&l1);
    lights.push_back(&l2);
    return lights;
}

float archSDF(vec3f pos){
    return differenceSDF(boxSDF(pos,{0.1f,0.6f,1.2f}),sphereSDF(pos , 0.4f));
}

vec2f sceneSDF(vec3f pos) {
    float d_plane = boxSDF(pos, {10.0f,0.001f,10.0f});
    float d_sphere1 = sphereSDF(pos + vec3f{0.6f,-0.5f,1.5f}, 0.5f);
    float d_sphere2 = sphereSDF(pos + vec3f{-0.6f,-0.5f,1.5f}, 0.5f);

    mat4f rot = inverse(rotateY(0.5f));
    auto pos_arc =  pos + vec3f{1.5f,-0.6f,4.0f};
    auto pos_arc4 = vec4f{pos_arc.x, pos_arc.y,pos_arc.z,1.0f};
    auto pos_arc_rot = rot * pos_arc4;
    float d_arch = archSDF(vec3f{pos_arc_rot.x,pos_arc_rot.y,pos_arc_rot.z});

    if(d_plane < d_sphere2)
        if(d_plane < d_sphere1)
            if(d_plane < d_arch)  return {d_plane, MAT_PLANE};
            else return {d_arch, MAT_CUBE};
        else if(d_sphere1 < d_arch)  return {d_sphere1, MAT_SPHERE};
        else return {d_arch, MAT_CUBE};
    else if(d_sphere2 < d_sphere1)
        if(d_sphere2 < d_arch)  return {d_sphere2, MAT_SPHERE};
        else return {d_arch, MAT_CUBE};
    else if(d_sphere1 < d_arch)  return {d_sphere1, MAT_SPHERE};
    else return {d_arch, MAT_CUBE};;
}
