//
// Created by Dario Montagnini on 08/01/18.
// code structure inspired by http://iquilezles.org/www/articles/filteringrm/filteringrm.htm
//

#include "../yocto/yocto_gl.h"
#include "scene_sdf.h"

#define NTHREADS 4

using namespace ygl;

float EPSILON = 0.001f;
float RAY_MAX = 100.0f;
float RAY_MIN = 0.01f;
int MAX_STEPS = 100;

vec3f gradient(vec3f pos){
    return normalize(vec3f{fScene({pos.x + EPSILON, pos.y, pos.z}).x - fScene({pos.x - EPSILON, pos.y, pos.z}).x,
                           fScene({pos.x, pos.y + EPSILON, pos.z}).x - fScene({pos.x, pos.y - EPSILON, pos.z}).x,
                           fScene({pos.x, pos.y, pos.z + EPSILON}).x - fScene({pos.x, pos.y, pos.z - EPSILON}).x});
}

ray3f eval_ray(camera &cam, vec2f uv) {
    auto h = 2 * std::tan(cam.yfov / 2.0f);
    auto w = h * cam.aspect;
    auto o = cam.frame.o;
    auto q = cam.frame.x * w * (uv.x - 0.5f) + cam.frame.y * h * (uv.y - 0.5f) - cam.frame.z + cam.frame.o;
    return ray3f{cam.frame.o, normalize(q - o), RAY_MIN, RAY_MAX};
}


vec2f intersect(ray3f &ray) {

    float tot_dist = 0.0f;
    float dist = 0.0f;
    vec3f pos = ray.o + ray.d * ray.tmin;
    vec2f v;

    for (int step = 0; step < MAX_STEPS; step++) {

        v = fScene(pos);
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


//   http://iquilezles.org/www/articles/rmshadows/rmshadows.htm
bool shadow(ray3f &shadow_ray){

    float h;
    float t = shadow_ray.tmin;
    while(t < shadow_ray.tmax) {
        h = fScene(shadow_ray.o + shadow_ray.d * t).x;
        if( h < EPSILON )
            return true;
        t += h;
    }
    return false;
}

vec4f shade(vec3f &amb, ray3f &ray, std::vector<light_sdf*> lights) {

    vec2f v = intersect(ray);

    if(v.y != -1.0f) {

        auto pos = ray.d * v.x + ray.o;
        auto norm = gradient(pos);

        auto mat = materials(v.y,pos);

        auto kd = mat.kd;
        auto ks = mat.ks;
        auto kr = mat.kr;
        auto rs = mat.rs;

        auto bp = rs ? 2.0f / std::pow(rs, 4.0f) - 2.0f : 1e6f;

        auto color = kd*amb;

        for(light_sdf* l : lights) {
            auto light_direction = normalize(pos - l->pos);
            float ll = length(pos - l->pos);
            ray3f shadow_ray = ray3f{l->pos, light_direction, RAY_MIN, ll - RAY_MIN};

            if(shadow(shadow_ray)) continue;

            auto intensity = l->ke / (ll * ll);
            auto h = normalize(ray.d*-1 + light_direction * -1);

            color += kd * intensity * max(0.0f, dot(-light_direction, norm));
            color += ks * intensity * powf(max(0.0f, dot(norm, h)), bp);

        }

        if (kr != vec3f()){
            auto refl_dir = 2 * dot(norm, -ray.d ) * norm + ray.d;
            ray3f refl_ray = {pos + refl_dir*EPSILON, refl_dir, RAY_MIN, RAY_MAX};
            auto refl = shade(amb, refl_ray, lights);
            color+= vec3f{refl.x, refl.y, refl.z} * kr;
        }

        return {color.x, color.y, color.z, 1.0f};
    }

    return {0.0f,0.0f,0.0f,1.0f};

}

void compute(int start, int nt, int samples, std::vector<light_sdf *>& lights,
                   vec3f& amb, image4f& img, camera& cam){

    for(int j = start; j < img.height(); j += nt){
        for(int i = 0; i < img.width(); i++){

            for (int k = 0; k < samples; k++){
                for (int l = 0; l < samples; l++){
                    float u = (i + (l + 0.5f) / samples) / img.width();
                    float v = (img.height() - j + (k + 0.5f) / samples) / img.height();

                    auto ray = eval_ray(cam, {u,v});
                    img.at(i, j) += shade(amb, ray, lights);
                }
            }
            img.at(i,j) *= 1.0f / (samples * samples);
        }
    }
}


image4f raymarcher(vec3f &amb, int resolution, int samples){

    auto lights = make_lights();
    auto cam = make_camera();
    load_textures();


    auto img = image4f((int)std::round(cam.aspect * resolution), resolution);

//    printf("width %d, height %d\n",img.width(),img.height());

    std::thread t[NTHREADS];
    for(int k = 0; k < NTHREADS; k++) {
        t[k] = std::thread(compute, k, NTHREADS, samples, std::ref(lights)
                , std::ref(amb), std::ref(img), std::ref(cam));
    }

    for(int k = 0; k < NTHREADS; k++) {
        t[k].join();
    }

    return img;
}


int main(int argc, char** argv) {

    // command line parsing
	auto parser = make_parser(argc, argv, "yraymarcher", "offline raymarcher");
	auto resolution = parse_opt<int>(parser, "--resolution", "-r", "vertical resolution", 720);
	auto samples = parse_opt<int>(parser, "--samples", "-s", "per-pixel samples", 1);
	auto amb = parse_opt<float>(parser, "--ambient", "-a", "ambient color", 0.1f);
	auto imageout = parse_opt<std::string>(parser, "--output", "-o", "output image", "../images/out.png");

    printf("hello world!\n");

    auto amb3f = vec3f(amb);

	auto hdr = raymarcher(amb3f, resolution, samples);

    save_image4f(imageout, hdr);
//    save_image4b(imageout,tonemap_image(hdr,0.0f,2.2f));
}
