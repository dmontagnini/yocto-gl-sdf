//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"

#define MAT_SPHERE 0.0f

// texture* txt_plane;

frame3f make_frame(vec3f ro, vec3f ta){
    frame3f f = frame3f();
    f.o = ro;
    f.z = normalize( ro-ta );
    f.x = normalize( cross( vec3f(0.0,1.0,0.0), f.z ) );
    f.y = normalize( cross( f.z, f.x ) );
    return f;
}


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
}


material materials(float id, vec3f pos){

    auto mat = material();
    if(id == MAT_SPHERE){
        mat.kd = vec3f{0.8f,0.8f,0.8f}; // sphere color
    }
    return mat;
}


std::vector<light_sdf*> make_lights(){
    auto lights = std::vector<light_sdf*>();

    auto l = new light_sdf();
    l->pos = vec3f{0.0f,6.0f,10.0f}; // light position
    l->ke = vec3f{100.0f, 100.0f, 100.0f}; // light intensity
    lights.push_back(l);

    return lights;
}


vec2f fScene(vec3f pos){
    auto d_sphere = fSphere(pos, 1.0f); // this function is defined in func_sdf.cpp
    return {d_sphere, MAT_SPHERE};
}
