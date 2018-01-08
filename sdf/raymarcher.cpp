//
// Created by Dario Montagnini on 08/01/18.
//

#include "../yocto/yocto_gl.h"
#include "../yocto/yocto_gl.cpp"

using namespace ygl;

float EPSILON = 0.001;

float shereSDF(vec3f pos, float r){
    return length(pos) - r;
}

vec3f abs(vec3f v){
    return {abs(v.x),abs(v.y),abs(v.z)};
}

vec3f max(vec3f v, vec3f u){
    return {max(v.x,u.x),max(v.y,u.y),max(v.z,u.z)};
}

float intersectSDF(float d1, float d2){
    return max(d1,d2);
}

float unionSDF(float d1, float d2){
    return min(d1,d2);
}

float differenceSDF(float d1, float d2){
    return max(d1,-d2);
}


float boxSDF(vec3f pos, vec3f size){
    return length(max(abs(pos)-size,{0.0f,0.0f,0.0f}));
}

float sceneSDF(vec3f pos){
    return unionSDF(boxSDF(pos,{1.0f,1.0f,1.0f}),shereSDF(pos + vec3f{0.0f,-2.0f,-2.0f}, 1.2f));
}

vec3f gradient(vec3f pos){
    return normalize(vec3f{sceneSDF({pos.x + EPSILON, pos.y, pos.z}) - sceneSDF({pos.x - EPSILON, pos.y, pos.z}),
                           sceneSDF({pos.x , pos.y+ EPSILON, pos.z}) - sceneSDF({pos.x , pos.y - EPSILON, pos.z}),
                           sceneSDF({pos.x, pos.y, pos.z + EPSILON}) - sceneSDF({pos.x, pos.y, pos.z - EPSILON})});
}

image4f raymarcher(){

    auto img = image4f(1024,720);
    vec3f o = {3.0f,3.0f,3.0f};
    vec3f t = {0.0f,0.0f,0.0f};
    vec3f up_dir = {0.0f,1.0f,0.0f};

    vec3f cam_dir = normalize(t - o);
    vec3f cam_right = normalize(cross(up_dir,o));
    vec3f cam_up = normalize(cross(cam_dir,cam_right));

    float aspect_ratio = (1024.0f/720.0f);
    vec4f RED = {1.0f,1.0f,1.0f,1.0f};

    for(int j = 0; j < img.height(); j++){
        for(int i = 0; i < img.width(); i++){

            float u = -1.0f + 2.0f * (i + 0.5f) / img.width();
            float v = -1.0f + 2.0f * (j + 0.5f) / img.height();
            u *= aspect_ratio;
//            float u = (i + 0.5f) / img.width();
//            float v = ((img.height() - j) + 0.5f) / img.height();
//            u = -1.0f + 2.0f * u * (1024.0f/720.0f);
//            v = -1.0f + 2.0f * v;
            vec3f ray = normalize(cam_right * u + cam_up * v + cam_dir);

            float tot_dist = 0.0f;
            vec3f pos = o;
            float dist = 0.001f;

            for(int step = 0; step < 100; step++){
                if(dist < 0.001f || tot_dist > 20.0) break;

                dist = sceneSDF(pos);
                tot_dist += dist;
                pos += dist * ray;
            }

//            printf("%f\n",dist);
            if(dist < 0.001f) {
                float diffuse = max(0.0f,dot(-ray,gradient(pos)));
                float specular = pow(diffuse,32.0f);
                float ds = diffuse + specular;
                img.at(i, j) = {RED.x*ds, RED.y*ds, RED.z*ds, 1.0f};
            }
            else
                img.at(i,j) = {0.0f,0.0f,0.0f,1.0f};
        }
    }

    return img;
}


int main(int argc, char** argv) {
    // command line parsing
    //auto parser = make_parser(argc, argv, "ytrace", "offline oath tracing");


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

    auto hdr = raymarcher();
    save_image4f("../tests/img.hdr", hdr);
}
