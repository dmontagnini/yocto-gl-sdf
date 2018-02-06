//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"
//
//#define MAT_PLANE 0.0
//#define MAT_STRUCT 1.0

#define MAT_SPHERE 0.0f

texture *txt_plane;
texture *txt_struct;


frame3f make_frame(vec3f ro, vec3f ta){
    frame3f f = frame3f();
    f.o = ro;
    f.z = normalize( ro-ta );
    f.x = normalize( cross( vec3f(0.0,1.0,0.0), f.z ) );
    f.y = normalize( cross( f.z, f.x ) );
    return f;
}


//camera make_camera() {
//    vec3f ro = {6.5f,3.5f,5.5f};
////    vec3f ro = {10.0f,2.0f,0.0f};
//    vec3f ta = vec3f(0.0f,0.0f,0.0f);
//    auto cam = camera();
//    cam.frame = make_frame(ro,ta);
//    cam.yfov = 0.26f * 2.0f;
//    return cam;
//
//}

camera make_camera() {
    vec3f ro = {0.0f,0.0f,5.0f}; // camera position
    vec3f ta = vec3f(0.0f,0.0f,0.0f); // point where the camera is looking at
    auto cam = camera();
    cam.frame = make_frame(ro,ta); // this is a function that returns the corresponding frame
    cam.yfov = 0.26f * 2.0f; // camera field of view
    return cam;
}

vec3f eval_texture(texture *txt, vec2f uv){
    auto txt_info = texture_info();
    txt_info.txt = txt;
    auto color = eval_texture(txt_info,uv);
    return {color.x,color.y,color.z};
}


void load_textures(){
//    txt_plane = new texture{"street.png"};
//    txt_plane->ldr = load_image4b("../tests/street.png");
//    txt_struct = new texture{"brick.png"};
//    txt_struct->ldr = load_image4b("../tests/brick.png");
}

//material materials(float id, vec3f pos){
//
//    auto mat = material();
//
//    if(id == MAT_PLANE){
//        mat.kd = vec3f{0.3f,0.3f,0.3f};
////        mat.kr = vec3f{0.3f,0.3f,0.3f};
//        mat.kd *= eval_texture(txt_plane,{mod(pos.x,1.0f),mod(pos.z,1.0f)});
//    }
//    if(id == MAT_STRUCT){
////        mat.kd = vec3f{0.27f,0.09f,0.05f};
//        mat.kd = vec3f{0.27f,0.09f,0.05f}*1.5f;
//        mat.kd *= eval_texture(txt_struct,{mod(pos.x*pos.z,1.0f),mod(pos.y,1.0f)});
//
//    }
//
//    return mat;
//
//}

material materials(float id, vec3f pos){

    auto mat = material();
    if(id == MAT_SPHERE){
        mat.kd = vec3f{0.8f,0.8f,0.8f}; // sphere color
    }
    return mat;
}


//std::vector<light_sdf*> make_lights(){
//    auto lights = std::vector<light_sdf*>();
//
//    //  cam light
//    auto lc = new light_sdf();
//    lc->pos = vec3f{0.0f,6.0f,10.0f};
////    lc->pos = vec3f{10.0f,2.0f,0.0f};
//    lc->ke = vec3f{30.0f, 30.0f, 30.0f};
//    lights.push_back(lc);
//
////  light1
//    auto l1 = new light_sdf();
//    l1->pos = vec3f(2.0f,6.0f,-10.0f);
//    l1->ke = vec3f{100.0f, 100.0f, 100.0f};
//    lights.push_back(l1);
//
////  light2
//    auto l2 = new light_sdf();
//    l2->pos = vec3f(-2.0f,6.0f,10.0f);
//    l2->ke = vec3f{100.0f, 100.0f, 100.0f};
//    lights.push_back(l2);
//
//    return lights;
//}

std::vector<light_sdf*> make_lights(){
    auto lights = std::vector<light_sdf*>();

    auto l = new light_sdf();
    l->pos = vec3f{0.0f,6.0f,10.0f}; // light position
    l->ke = vec3f{100.0f, 100.0f, 100.0f}; // light intensity
    lights.push_back(l);

    return lights;
}

//vec2f fScene(vec3f pos) {
//
//    auto d_plane = fBox(pos, {20.0f,0.001f,20.0f});
//
//    pos.z = pMirror(pos.z,1.5);
//
//    float d_wall = fBox2(vec2f{pos.y,pos.z} + vec2f(-1.1f,0.0f), vec2f{1.1f,0.1});
//    d_wall = min(fBox2(vec2f{pos.y,pos.z} + vec2f(-1.1f,0.0f), vec2f{0.15f,0.14}), d_wall);
//    d_wall = min(fBox2(vec2f{pos.y,pos.z} + vec2f(-1.6f,0.0f), vec2f{0.03f,0.14}), d_wall);
//    d_wall = min(fBox2(vec2f{pos.y,pos.z} + vec2f(-1.5f,0.0f), vec2f{0.03f,0.14}), d_wall);
//    d_wall = min(fBox2(vec2f{pos.y,pos.z} + vec2f(-1.4f,0.0f), vec2f{0.03f,0.14}), d_wall);
//    d_wall = fOpUnionRound(fBox2(vec2f{pos.y,pos.z} + vec2f(-2.0f,0.0f), vec2f{0.05f,0.2}), d_wall,0.02f);
//
//    pos = vec3f(pMod1(pos.x,1.1),pos.y,pos.z);
//
//    pos.x = abs(pos.x) + 0.15f;
//
//    float d_window = fBox(pos + vec3f(0.0f,-0.5f,0.0f), vec3f{0.5f,0.8f,0.3f});
//    d_window = fUnion(fCylinder(vec3f{pos.x,pos.z,pos.y}+ vec3f(0.0f,0.0f,-1.3f),0.5f,0.3),d_window);
//
//    float d_building = fOpDifferenceChamfer(d_wall,d_window,0.03);
//
//    if (d_plane < d_building) return {d_plane, MAT_PLANE};
//    else return {d_building, MAT_STRUCT};
//
//}

vec2f fScene(vec3f pos){
    auto d_sphere = fSphere(pos, 1.0f); // this function is defined in func_sdf.cpp
    return {d_sphere, MAT_SPHERE};
}
