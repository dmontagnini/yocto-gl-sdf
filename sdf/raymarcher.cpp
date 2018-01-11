//
// Created by Dario Montagnini on 08/01/18.
//

#include "../yocto/yocto_gl.h"
#include "distance_functions.h"
#include "math_sdf.h"
#include "scene_sdf.h"

using namespace ygl;

float EPSILON = 0.001f;
float RAY_MAX = 800.0f;
vec4f RED = {1.0f,1.0f,1.0f,1.0f};
float RAY_MIN = 0.001f;
int MAX_STEPS = 100;

vec3f gradient(vec3f pos){
    return normalize(vec3f{sceneSDF({pos.x + EPSILON, pos.y, pos.z}).x - sceneSDF({pos.x - EPSILON, pos.y, pos.z}).x,
                           sceneSDF({pos.x , pos.y+ EPSILON, pos.z}).x - sceneSDF({pos.x , pos.y - EPSILON, pos.z}).x,
                           sceneSDF({pos.x, pos.y, pos.z + EPSILON}).x - sceneSDF({pos.x, pos.y, pos.z - EPSILON}).x});
}

ray3f eval_ray(camera &cam, vec2f uv) {
    auto h = 2 * std::tan(cam.yfov / 2.0f);
    auto w = h * cam.aspect;
    auto o = cam.frame.o;
    auto q = cam.frame.x * w * (uv.x -0.5f) + cam.frame.y * h * (uv.y -0.5f) - cam.frame.z + cam.frame.o;
   // vec3f ray = normalize(cam.frame.x * uv.x + cam.frame.y * uv.y + cam.frame.z);
    return ray3f{cam.frame.o, normalize(q - o), RAY_MIN, RAY_MAX};
}

vec2f intersect(ray3f &ray) {
    float tot_dist = 0.0f;
    float dist = ray.tmin;
    vec3f pos = ray.o;
    vec2f v;
    for(int step = 0; step < MAX_STEPS; step++){
        if(dist < ray.tmin || tot_dist > ray.tmax) break;

        v = sceneSDF(pos);
        dist = v.x;
        tot_dist += dist;
        pos += dist * ray.d;
    }

    if(dist < ray.tmin) {
        return {tot_dist, v.y};
    }
    else return {tot_dist,-1.0f};
}

vec4f shade(vec3f &amb, ray3f &ray) {



//    float tot_dist = 0.0f;
//    float dist = ray.tmin;
//    vec3f pos = ray.o;
//
//    for(int step = 0; step < MAX_STEPS; step++){
//        if(dist < ray.tmin || tot_dist > ray.tmax) break;
//
//        dist = sceneSDF(pos);
//        tot_dist += dist;
//        pos += dist * ray.d;
//    }

    // calcola luci
    // calcola colore materia

    vec2f v = intersect(ray);

    if(v.y != -1.0f) {
        auto pos = ray.d * v.x + ray.o;
        auto norm = gradient(pos);
        auto kd = make_materials(v.y);
        auto color = kd*amb;

        for(light_sdf* l : set_lights()) {
            auto light_direction = normalize(l->pos - pos);
            auto ll = length(l->pos - pos);
            ray3f shadow_ray = ray3f{pos, light_direction, RAY_MIN*3, ll};
            //printf("%f %f %f\n", l->pos.x, l->pos.y, l->pos.z);
            //if(intersect(shadow_ray).y != -1.0f) continue;
           // printf("%f\n", intersect(shadow_ray).y);
            float diffuse = max(0.0f,dot(-ray.d,norm));
            //float specular = pow(diffuse,32.0f);
            float ds = diffuse; //+ specular;

            //auto intensity = l->ke / ( ll * ll);
            color += color * ds;
           // color += kd * intensity * max(0.0f, dot(light_direction, norm));
        }
        return {color.x, color.y, color.z, 1.0f};
    }
    else
        return {0.0f,0.0f,0.0f,1.0f};
}

image4f raymarcher(vec3f &amb,camera &cam, int resolution){

    auto img = image4f((int)std::round(cam.aspect * resolution), resolution);

    for(int j = 0; j < img.height(); j++){
        for(int i = 0; i < img.width(); i++){

            float u = (i + 0.5f) / img.width();
            float v = (img.height() - j + 0.5f) / img.height();
//            float u = -1.0f + 2.0f * (i + 0.5f) / img.width();
//            float v = -1.0f + 2.0f * (j + 0.5f) / img.height();
           // u *= cam.aspect;
            auto ray = eval_ray(cam, {u,v});
            img.at(i, j) = shade(amb, ray);
        }
    }

    return img;
}


int main(int argc, char** argv) {
    // command line parsing
    //auto parser = make_parser(argc, argv, "ytrace", "offlin oath tracing");


    print("hello world!\n");

    // load scene
//    printf("loading scene %s\n", scenein.c_str());
//    auto scn = load_scene(scenein);

    // create bvh
//    printf("creating bvh\n");
//    build_bvh(scn, false);

    // raytrace
//    printf("tracing scene\n");
//    auto hdr = raytrace(scn, vec3f{amb, amb, amb}, resolution, samples);

    // tonemap and save
//    printf("saving image %s\n", imageout.c_str());
//    save_hdr_or_ldr(imageout, hdr);

//    vec3f o = {0.0f,5.0f,10.0f};
//    vec3f t = {0.0f,0.0f,0.0f};
//    vec3f up_dir = {0.0f,1.0f,0.0f};

//    vec3f cam_dir = normalize(t - o);
//    vec3f cam_right = normalize(cross(up_dir,o));
//    vec3f cam_up = normalize(cross(cam_dir,cam_right));

   // auto cam = camera();
//    cam.frame.x = cam_right;
//    cam.frame.y = cam_up;
//    cam.frame.z = cam_dir;

//    cam.frame.x = {1.0f, 0.0f, 0.0f};
//    cam.frame.y = {0.0f, 1.0f, 0.0f};
//    cam.frame.z = {0.0f, 0.0f, 1.0f};



//    cam.yfov = 1.0f;
//    printf("%f %f %f", prova.x, prova.y, prova.z);
    vec3f amb = set_environment();

    camera cam = set_camera();



    auto hdr = raymarcher(amb, cam, 720);
    save_image4f("../tests/img.hdr", hdr);
}
