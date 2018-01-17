//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"


#define MAT_PLANE 0.0
#define MAT_SPHERE 1.0
#define MAT_CUBE 2.0

#define MAT_TEMPLE 4.0

camera make_camera() {

    auto cam = camera();

    cam.frame.o = {0.0f,4.0f,10.0f};
    cam.frame.x = {1.0f,0.0f,0.0f};
    cam.frame.y = {0.0f, 0.957826f, -0.287348f};
    cam.frame.z = {0.0f, 0.287348f, 0.957826f};
    cam.yfov = 0.261799f;

    return cam;
}


std::vector<material*> make_materials() {
    auto mats = std::vector<material *>();

    // plane material
    auto mat_plane = new material();
    mat_plane->kd = vec3f{0.5f,0.5f,0.5f};
    mat_plane->kr = vec3f{0.2f,0.2f,0.2f};
    mat_plane->kd_txt.txt = new texture{"wood.png"};
    mat_plane->kd_txt.txt->ldr = load_image4b("../tests/wood.png");
    mats.push_back(mat_plane);

    // sphere material
    auto mat_sphere = new material();
    mat_sphere->kd = vec3f{0.96f, 0.96f, 0.96f};
    mat_sphere->kr = vec3f{0.0f,0.0f,0.0f};
    mat_sphere->ks = vec3f{0.8f,0.8f,0.8f};
    mat_sphere->rs = 0.3f;
    mat_sphere->op = 0.2f;
    mat_sphere->kd_txt.txt = new texture{"colored.png"};
    mat_sphere->kd_txt.txt->ldr = load_image4b("../tests/colored.png");
    mats.push_back(mat_sphere);

    // cube material
    auto mat_cube = new material();
    mat_cube->kd = vec3f{0.6f,0.6f,0.6f};
    mat_cube->kr = vec3f{0.0f,0.0f,0.0f};
    mat_cube->ks = vec3f{0.8f,0.8f,0.8f};
    mat_cube->rs = 0.3f;
    mat_cube->op = 0.2f;
    mat_cube->kd_txt.txt = new texture{"colored.png"};
    mat_cube->kd_txt.txt->ldr = load_image4b("../tests/colored.png");
    mats.push_back(mat_cube);

    return mats;
}

std::vector<light_sdf*> make_lights(){
    auto lights = std::vector<light_sdf*>();

    // light1
    auto l1 = new light_sdf();
    l1->pos = vec3f{1.4f,8.0f,6.0f};
    l1->ke = vec3f{100.0f, 100.0f, 100.0f};
    lights.push_back(l1);

    // light2
    auto l2 = new light_sdf();
    l2->pos = vec3f{-1.4f,8.0f,6.0f};
    l2->ke = vec3f{100.0f, 100.0f, 100.0f};
    lights.push_back(l2);

    return lights;
}

vec2f texture_mapping(vec3f pos, float id) {
    vec2f uv = vec2f();
    if (id == 0.0){
        uv.x = fmod(abs(pos.x),1.0f);
        uv.y = fmod(abs(pos.z),1.0f);
    }
    else if (id == 1.0) {
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
        uv.x = 9.0f/32.0f;
        uv.y = 1.0f/32.0f;
    }
    return uv;
}


float archSDF(vec3f pos){
    return differenceSDF(udBox(pos,{0.1f,0.6f,1.2f}),sdSphere(pos , 0.4f));
}

float templeSDF(vec3f pos){
    float c1 = sdCappedCylinder(pos + vec3f{-0.2f,-0.6f,0.8f},vec2f(0.1f,0.6f));
    float c2 = sdCappedCylinder(pos + vec3f{0.2f,-0.6f,0.8f},vec2f(0.1f,0.6f));
    float c3 = sdCappedCylinder(pos + vec3f{-0.6f,-0.6f,0.8f},vec2f(0.1f,0.6f));
    float c4 = sdCappedCylinder(pos + vec3f{0.6f,-0.6f,0.8f},vec2f(0.1f,0.6f));
    float c5 = sdCappedCylinder(pos + vec3f{-0.2f,-0.6f,-0.8f},vec2f(0.1f,0.6f));
    float c6 = sdCappedCylinder(pos + vec3f{0.2f,-0.6f,-0.8f},vec2f(0.1f,0.6f));
    float c7 = sdCappedCylinder(pos + vec3f{-0.6f,-0.6f,-0.8f},vec2f(0.1f,0.6f));
    float c8 = sdCappedCylinder(pos + vec3f{0.6f,-0.6f,-0.8f},vec2f(0.1f,0.6f));
    float rect1 = udBox(pos, vec3f{2.0f,0.1f,1.0f});
    return unionSDF(unionSDF(unionSDF(c5,c6),unionSDF(c7,c8)),
    unionSDF(unionSDF(unionSDF(c1,c2),unionSDF(c3,c4)),rect1));
}



vec2f sceneSDF(vec3f pos) {
    float d_plane = udBox(pos, {10.0f,0.001f,10.0f});
    float d_sphere = sdSphere(pos + vec3f{0.6f,-0.5f,1.5f}, 0.5f);
    float d_cube = udBox(pos + vec3f{-0.6f,-0.5f,1.5f}, { 0.5f, 0.5f, 0.5f});

//    mat4f rot = inverse(rotateY(0.5f));
//    auto pos_arc =  pos + vec3f{1.5f,-0.6f,4.0f};
//    auto pos_arc4 = vec4f{pos_arc.x, pos_arc.y,pos_arc.z,1.0f};
//    auto pos_arc_rot = rot * pos_arc4;
//    float d_arch = archSDF(vec3f{pos_arc_rot.x,pos_arc_rot.y,pos_arc_rot.z});
//    float d_temple = templeSDF(pos + vec3f{0.0f,0.0f,3.0f});
//
//    return {d_plane, MAT_PLANE};

    if(d_plane < d_cube)
        if (d_plane < d_sphere) return {d_plane, MAT_PLANE};
        else return {d_sphere, MAT_SPHERE};
    else if (d_cube < d_sphere) return {d_cube, MAT_CUBE};
    else return {d_sphere, MAT_SPHERE};

//    if(d_plane < d_cube)
//        if(d_plane < d_sphere1)
//            if(d_plane < d_cube)  return {d_plane, MAT_PLANE};
//            else return {d_cube, MAT_CUBE};
//        else if(d_sphere1 < d_cube)  return {d_sphere1, MAT_SPHERE};
//        else return {d_cube, MAT_CUBE};
//    else if(d_sphere2 < d_sphere1)
//        if(d_sphere2 < d_cube)  return {d_sphere2, MAT_SPHERE};
//        else return {d_cube, MAT_CUBE};
//    else if(d_sphere1 < d_cube)  return {d_sphere1, MAT_SPHERE};
//    else return {d_cube, MAT_CUBE};
//    if(d_plane < d_temple) return {d_plane, MAT_PLANE};
//    else return {d_temple, MAT_TEMPLE};
}
