//
// Created by Dario Montagnini on 08/01/18.
//

#include "../yocto/yocto_gl.h"
#include "distance_functions.h"
#include "math_sdf.h"
#include "scene_sdf.h"

using namespace ygl;

float EPSILON = 0.001f;
float RAY_MAX = 100.0f;
float RAY_MIN = 0.01f;
int MAX_STEPS = 100;

image4b* textg;
image4b* textc;


vec3f eval_texture(image4b *img, vec2f uv){
    float s = uv.x * img->width();
    float t = uv.y * img->height();

    int i = (int)s;
    int j = (int)t;

    float wi = s-i;
    float wj = t-j;

    int i1 = (i+1) % img->width();
    int j1 = (j+1) % img->height();

    auto c1 = img->at(i,j);
    auto c2 = img->at(i1,j);
    auto c3 = img->at(i,j1);
    auto c4 = img->at(i1,j1);

    return vec3f{c1.x/255.0f, c1.y/255.0f , c1.z/255.0f} * (1-wi)*(1-wj) +
           vec3f{c2.x/255.0f, c2.y/255.0f, c2.z/255.0f} * wi * (1-wj)+
           vec3f{c3.x/255.0f, c3.y/255.0f, c3.z/255.0f} * (1-wi) * wj +
           vec3f{c4.x/255.0f, c4.y/255.0f, c4.z/255.0f} * wi * wj;
}

vec3f gradient(vec3f pos){
    return normalize(vec3f{sceneSDF({pos.x + EPSILON, pos.y, pos.z}).x - sceneSDF({pos.x - EPSILON, pos.y, pos.z}).x,
                           sceneSDF({pos.x , pos.y+ EPSILON, pos.z}).x - sceneSDF({pos.x , pos.y - EPSILON, pos.z}).x,
                           sceneSDF({pos.x, pos.y, pos.z + EPSILON}).x - sceneSDF({pos.x, pos.y, pos.z - EPSILON}).x});
}

ray3f eval_ray(camera &cam, vec2f uv) {
    auto h = 2 * std::tan(cam.yfov / 2.0f);
    auto w = h * cam.aspect;
    auto o = cam.frame.o;
    auto q = cam.frame.x * w * (uv.x - 0.5f) + cam.frame.y * h * (uv.y - 0.5f) - cam.frame.z + cam.frame.o;
   // vec3f ray = normalize(cam.frame.x * uv.x + cam.frame.y * uv.y + cam.frame.z);
    return ray3f{cam.frame.o, normalize(q - o), RAY_MIN, RAY_MAX};
}

vec2f intersect(ray3f &ray) {

    float tot_dist = 0.0f;
    float dist = 0.0f; //ray.tmin;
    vec3f pos = ray.o + ray.d * ray.tmin;
    vec2f v;

    for (int step = 0; step < MAX_STEPS; step++) {
       // if (dist < 0.001f || tot_dist > ray.tmax) break;

        v = sceneSDF(pos);
        dist = v.x;
        tot_dist += dist;
        pos += dist * ray.d;

        if (dist < EPSILON || tot_dist > ray.tmax) break;
    }

    if (dist < EPSILON) {
        return {tot_dist, v.y};
    }
    else return {tot_dist, -1.0f};

}

bool shadow(ray3f &shadow_ray){

    float h = 0.0f;
    float t = shadow_ray.tmin;
    //printf("tmax: %f", shadow_ray.tmax);
    while(t < shadow_ray.tmax) {
        h = sceneSDF(shadow_ray.o + shadow_ray.d * t).x;
        //printf("t: %f    h: %f\n", t, h);
        if( h < EPSILON ) {
            //printf("true \n");
            return true;
        }
        t += h;
    }
    //printf("\n");
    //fflush(stdout);
    return false;
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

    auto rs = 0.3f;
    auto bp = rs ? 2 / std::pow(rs, 4.0f) - 2 : 1e6f;

    if(v.y != -1.0f) {
        auto pos = ray.d * v.x + ray.o;
        auto norm = gradient(pos);
        auto kd = make_materials(v.y);

        auto ks = vec3f{0.8f, 0.8f, 0.8f};
        auto kr = vec3f{};

        if(v.y == 3.0f){
            ks = vec3f{0.0f, 0.0f, 0.0f};
            kr = vec3f{0.4f,0.4f,0.4f};
        }


        auto texcoord = texture_mapping(pos, v.y);
            //printf("X: %f Y: %f\n", texcoord.x, texcoord.y);
        if(v.y == 1.0f) kd *= eval_texture(textc,texcoord);
        else if(v.y == 2.0f) kd *= eval_texture(textc,texcoord);
        else if(v.y == 3.0f) kd *= eval_texture(textg,texcoord);


        auto color = kd*amb;

        auto lights = std::vector<light_sdf*>();
//        auto position = vec3f{-10.0f,5.0f,-1.5f};
        auto position = vec3f{1.4f,8.0f,6.0f};
        auto ke = vec3f{100.0f, 100.0f, 100.0f};
        auto l1 = light_sdf{position, ke};
        auto pos2 = vec3f{-1.4f,8.0f,6.0f};
        auto l2 = light_sdf{pos2, ke};
        lights.push_back(&l1);
        lights.push_back(&l2);

        for(light_sdf* l : lights) {
            auto light_direction = normalize(pos - l->pos);
            float ll = length(pos - l->pos);
            ray3f shadow_ray = ray3f{l->pos, light_direction, RAY_MIN, ll - RAY_MIN};
            //printf("%f %f %f\n", l->pos.x, l->pos.y, l->pos.z);
            //printf("sopra\n");

            if(shadow(shadow_ray)) continue;
            //printf("sotto\n");
//            intersect(shadow_ray);
//            float diffuse = max(0.0f,dot(-ray.d,norm));
//            float specular = pow(diffuse,32.0f);
//            float ds = diffuse; //+ specular;

            //printf("ll: %f\n", ll);
            auto intensity = l->ke / ( ll * ll);

            auto h = normalize(ray.d*-1 + light_direction * -1);
//            printf("intensity: %f %f %f\n", intensity.x, intensity.y, intensity.z);
//            printf("ke: %f %f %f\n", l->ke.x, l->ke.y, l->ke.z);

            //color = color * ds
            color += kd * intensity * max(0.0f, dot(-light_direction, norm));

            if (v.y !=3.0) color+= ks * intensity * powf(max(0.0f, dot(norm, h)), bp);

        }

        if (v.y==3.0){
            auto refl_dir = 2 * dot(norm, -ray.d ) * norm + ray.d;
            ray3f refl_ray = {pos + refl_dir*EPSILON, refl_dir, RAY_MIN, RAY_MAX};
//            auto a = vec3f();
            auto refl = shade(amb, refl_ray);
            color+= vec3f{refl.x, refl.y, refl.z} * kr;
        }

        return {color.x, color.y, color.z, 1.0f};
    }
    else
        return {0.0f,0.0f,0.0f,1.0f};
}




image4f raymarcher(vec3f &amb,camera &cam, int resolution, int samples){

    auto img = image4f((int)std::round(cam.aspect * resolution), resolution);

    for(int j = 0; j < img.height(); j++){
        for(int i = 0; i < img.width(); i++){
            for (int k=0; k<samples; k++){
                for (int l=0; l<samples; l++){
                    float u = (i +(l + 0.5f)/samples)/ img.width();
                    float v = (img.height() - j + (k + 0.5f)/samples) / img.height();

                    auto ray = eval_ray(cam, {u,v});

                    img.at(i, j) += shade(amb, ray);
                }
            }
            img.at(i,j) *= 1.0f/ (samples * samples);
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

    auto samples = 1;

    auto txtg = load_image4b("../tests/grid.png");
    auto txtc = load_image4b("../tests/colored.png");

    textc = &txtc;
    textg = &txtg;

//    textg = new texture();
//    textg->name="grid";
//    textg->path="../tests/grid.png";

    auto hdr = raymarcher(amb, cam, 720,samples);
    save_image4f("../tests/img.hdr", hdr);
}
