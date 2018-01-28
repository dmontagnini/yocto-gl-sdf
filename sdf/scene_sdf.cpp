//
// Created by Dario Montagnini on 11/01/18.
//

#include "scene_sdf.h"


#define MAT_HEAD_MASHROOM 0.0
#define MAT_NECK_MASHROOM 1.0
#define MAT_LADY_BODY 2.0
#define MAT_LADY_HEAD 3.0
#define MAT_LADY_LEGS 4.0

#define MAT_SILVER 5.0

//texture* txt1;

//#define MAT_PLANE 0.0
//#define MAT_SPHERE 1.0
//#define MAT_CUBE 2.0


camera make_camera() {

    vec3f ro = vec3f(-300.0f,150.0f,0.0f);

//    vec3f ro = vec3f(-160.0f,10.0f,0.0f);

    vec3f ta = vec3f(0.0f,0.0f,0.0f);

    vec3f w = normalize( ro-ta );
    vec3f u = normalize( cross( vec3f(0.0,1.0,0.0), w ) );
    vec3f v = normalize( cross( w, u ) );

    auto cam = camera();
////    cam.frame.o = {0.0f,8.0f,30.0f};
//    cam.frame.o = {0.0f,6.0f,-20.0f};
////    cam.frame.o = {0.0f,4.0f,10.0f};
//    cam.frame.x = {1.0f,0.0f,0.0f};
//    cam.frame.y = {0.0f, 0.957826f, -0.287348f};
//    cam.frame.z = {0.0f, -0.287348f, -0.957826f};
//    cam.yfov = 0.261799f;
    cam.frame.o = ro;
    cam.frame.x = u;
    cam.frame.z = w;
    cam.frame.y = v;
    cam.yfov = 0.261799f*4.0f;

//    txt1 = new texture{"organic.png"};
//    txt1->ldr = load_image4b("../tests/organic.png");

    return cam;
}

vec3f worldToMushrom(vec3f pos )
{
    vec3f qos = pos;
    auto qosxy = (mat2f{vec2f(60,11),vec2f(-11,60)}/61.0) * vec2f{qos.x,qos.y};
    qos.x = qosxy.x;
    qos.y = qosxy.y;
    qos.y += 0.03*sin(3.0*qos.z - 2.0*sin(3.0*qos.x));
    qos.y -= 0.4;
    return qos;
}


vec3f worldToLadyBug(vec3f pos){
    pos = 4.0*(pos - vec3f(-0.0f,3.2f-0.6f,-0.57f));
    pos = rotateY(3.5f)*(rotateZ(0.49f)*(rotateX( -0.92f )*pos));
//    pos = rotateY(pif) * pos;
//    pos = rotateX(pif/4.0f) * pos;
    pos.y += 0.2;

    return pos;
}

float smoothstep(float x, float y, float k){
    k = clamp((k - x)/(y-x), 0.0f,1.0f);
    return k * k * (3 - 2 * k);
}

vec3f mix(vec3f a, vec3f b, float f)
{
    return {a.x * (1.0f - f) + f*b.x,
            a.y * (1.0f - f) + f*b.y,
            a.z * (1.0f - f) + f*b.z};
}

vec3f eval_texture(texture *img, vec2f uv){

    uv = abs(uv);
    uv = {fmodf(uv.x,1.0f),fmodf(uv.y,1.0f)};

    float s = uv.x * img->width();
    float t = uv.y * img->height();

    auto i = (int)fmodf(s,img->width());
    auto j = (int)fmodf(t,img->height());

    float wi = s - i;
    float wj = t - j;

    int i1 = (i + 1) % img->width();
    int j1 = (j + 1) % img->height();

    auto c1 = img->ldr.at(i, j);
    auto c2 = img->ldr.at(i1, j);
    auto c3 = img->ldr.at(i, j1);
    auto c4 = img->ldr.at(i1, j1);

    return vec3f{c1.x/255.0f, c1.y/255.0f , c1.z/255.0f} * (1-wi)*(1-wj) +
           vec3f{c2.x/255.0f, c2.y/255.0f, c2.z/255.0f} * wi * (1-wj)+
           vec3f{c3.x/255.0f, c3.y/255.0f, c3.z/255.0f} * (1-wi) * wj +
           vec3f{c4.x/255.0f, c4.y/255.0f, c4.z/255.0f} * wi * wj;
}

material materials(float id, vec3f pos){
//
    auto mat = material();
//    // head mashtroom material
//    if(id == MAT_HEAD_MASHROOM) {
//        auto m1 = pos - vec3f{0.0f,0.1f,0.0f};
//
//
//
//        auto qos = worldToMushrom(pos);
//        mat.kd = vec3f(0.28f, 0.20f, 0.16f);
//        mat.kd -= vec3f(smoothstep(0.4,0.9,eval_texture( txt1, vec2f{qos.x  * 0.8f,qos.z * 0.8f}).x)) * 0.2;
//        mat.kd = mix( vec3f(1.0f,0.85f,0.72f ), mat.kd, smoothstep(1.5,2.4,qos.y) );
//        mat.kd = mix( vec3f(0.20f,0.001f,0.008f ), mat.kd, smoothstep(1.5,1.65,qos.y) );
//        mat.kd -= 0.2*eval_texture( txt1, 0.1*vec2f{qos.x,qos.z});
//        mat.kd *= 0.5f;
//        mat.kd = max( mat.kd, vec3f(0.0) );
//
//        mat.kd += mat.kd*vec3f(0.3,0.6,0.0)*(1.0-smoothstep( 0.8, 1.4, length(m1-vec3f(0.5,1.1,-0.3f)) ));
//
//        mat.rs = 0.6;
////        matSSS = 1.0;
////        matOcc = smoothstep( 0.4,1.5,length(worldToLadyBug( pos ).xz) );
////        matRefOcc = matOcc;
////        mat.ks = vec3f(0.75,0.87,1.0);
//    }// neck mashtroom material
//    else if(id == MAT_NECK_MASHROOM) {
//
//        vec2f uv = vec2f( pos.y*0.5f, atan2(pos.x,pos.z)*(3.0f/3.14159f) );
//
//        mat.kd = vec3f(0.42,0.35,0.15);
//
//        float pa = smoothstep(0.3,0.8,pos.y);
//
//        auto t = eval_texture( txt1, 0.5*uv ).x;
//        mat.kd -= pa*0.2*vec3f(t);
//        mat.kd = max(vec3f(0.0), mat.kd);
//
//        mat.kd *= 0.22;
//        mat.kd = clamp(  mat.kd, 0.0, 1.0 );
//
//        mat.rs = 0.7f;
//
////        matOcc = clamp( (pos.y-0.5)/1.3,0.0,1.0);
////        matOcc = matOcc*matOcc;
////        matOcc *= clamp( 1.0-(pos.y-1.2)/1.2,0.0,1.0);
////        matOcc = matOcc*0.5 + 0.5*matOcc*matOcc;
////        matRefOcc = matOcc;
////        mat.kr = vec3f(0.75,0.95,1.0);
//    } // lady bug body material
//    else if(id == MAT_LADY_BODY){
//
//        vec3f qos = worldToLadyBug( pos );
//
//        // red
//        mat.kd = vec3f(0.50,0.008,0.0);
////
//        float f = eval_texture( txt1, 0.1*vec2f(qos.x,qos.z) ).x;
//        mat.kd = mix( mat.kd, vec3f(0.15,0.07,0.0), f*f );
////
//        qos.x = abs(qos.x);
//        vec2f uv = vec2f( atan2(qos.x,qos.y), 1.57f*qos.z )*0.1;
////
////        // white
//        float p = length( (vec2f{qos.x,qos.z}-vec2f(0.0,0.9)) * vec2f(0.5,1.0));
//        mat.kd = mix( mat.kd, vec3f(1.0,0.8,0.6)*0.6, 1.0f-smoothstep(0.09,0.14,p) );
////
//        // black
//        p = cos(uv.x*40.0f)*cos(uv.y*40.0f+1.57f);
//        mat.kd *= 1.0-smoothstep( 0.35f, 0.45f, p );
////
////
//        f = eval_texture( txt1, vec2f{qos.x,qos.z}*vec2f(0.8,0.1) ).x;
//        mat.kd *= 1.0 - 0.5*f;
//        f = eval_texture( txt1, 4.0*vec2f{qos.x,qos.z}).x;
//        mat.kd *= 1.0 - 0.99*f*f;
////
////
////        matColor *= 1.3;
////        matRough = 0.15;
////        matOcc = 0.6 + 0.4*smoothstep( 0.0,0.3,qos.y );
////        matRefOcc = 0.2 + 0.8*smoothstep( 0.0,0.35,qos.y );
//    } // lady bug head material
//    else if(id == MAT_LADY_HEAD){
//        mat.kd = vec3f(0.001f);
//    }// lady bug legs material
//    else if(id == MAT_LADY_LEGS){
//        mat.kd = vec3f(0.0,0.0,0.0);
//    }

    if(id == MAT_SILVER){
        mat.kd = vec3f{0.8f,0.8f,0.8f};
    }

    return mat;

    // plane material
//    auto mat_plane = new material();
//    mat_plane->kd = vec3f{0.5f,0.5f,0.5f};
//    mat_plane->kr = vec3f{0.4f,0.4f,0.4f};
//    mat_plane->kd_txt.txt = new texture{"grid.png"};
//    mat_plane->kd_txt.txt->ldr = load_image4b("../tests/grid.png");
//    mats.push_back(mat_plane);

    // sphere material
//    auto mat_sphere = new material();
//    mat_sphere->kd = vec3f{0.96f, 0.96f, 0.96f};
//    mat_sphere->kr = vec3f{0.0f,0.0f,0.0f};
//    mat_sphere->ks = vec3f{0.8f,0.8f,0.8f};
//    mat_sphere->rs = 0.3f;
//    mat_sphere->kd_txt.txt = new texture{"colored.png"};
//    mat_sphere->kd_txt.txt->ldr = load_image4b("../tests/colored.png");
//    mats.push_back(mat_sphere);

    // cube material
//    auto mat_cube = new material();
//    mat_cube->kd = vec3f{0.6f,0.6f,0.6f};
//    mat_cube->kr = vec3f{0.0f,0.0f,0.0f};
//    mat_cube->ks = vec3f{0.8f,0.8f,0.8f};
//    mat_cube->rs = 0.3f;
//    mat_cube->kd_txt.txt = new texture{"colored.png"};
//    mat_cube->kd_txt.txt->ldr = load_image4b("../tests/colored.png");
//    mats.push_back(mat_cube);

}


std::vector<light_sdf*> make_lights(){
    auto lights = std::vector<light_sdf*>();

//     light1
    auto l1 = new light_sdf();
    l1->pos = vec3f(vec3f(-100.0f,100.0f,0.0f));
    l1->ke = vec3f{500.0f, 500.0f, 500.0f};
    lights.push_back(l1);

//    light2
    auto l2 = new light_sdf();
    l2->pos = vec3f(100.0f,50.0f,0.0f);
    l2->ke = vec3f{500.0f, 500.0f, 500.0f};
    lights.push_back(l2);

    //     light1
    auto l3 = new light_sdf();
    l3->pos = vec3f(vec3f(0.0f,50.0f,100.0f));
    l3->ke = vec3f{500.0f, 500.0f, 500.0f};
    lights.push_back(l3);

//    light2
    auto l4 = new light_sdf();
    l4->pos = vec3f(0.0f,50.0f,-100.0f);
    l4->ke = vec3f{500.0f, 500.0f, 500.0f};
    lights.push_back(l4);



    //    light2
    auto l5 = new light_sdf();
    l5->pos = vec3f(0.0f,300.0f,0.0f);
    l5->ke = vec3f{500.0f, 500.0f, 500.0f};
    lights.push_back(l5);

    return lights;
}



vec2f mushroom(vec3f p){

    auto pos = worldToMushrom(p);
    float d1 = sdEllipsoid( pos, vec3f(0.0, 1.4,0.0), vec3f(0.8,1.0,0.8) );
    float d2 = sdEllipsoid( pos, vec3f(0.0, 0.5,0.0), vec3f(1.3,1.2,1.3) );
    float d_head = smax( d1, -d2, 0.1 );
    d_head *= 0.8;
    auto res = vec2f{d_head, MAT_HEAD_MASHROOM};


    //
    pos =  vec3f{p.x,p.y,p.z};
    pos.x += 0.3f*sin(pos.y) - 0.65f;
    float pa = sin( 20.0f*atan2(pos.z,pos.x) );
    vec2f se = sdLine( pos, vec3f(0.0f,2.0f,0.0f), vec3f(0.0f,0.0f,0.0f) );

    float tt = 0.25f - 0.1f*4.0f*se.y*(1.0f-se.y);

    float d_neck = se.x - tt;

    d_neck = smin( d_neck, sdEllipsoid( pos, vec3f(0.0, 1.7f - 2.0f*length({pos.x,pos.z}),0.0), vec3f(0.3,0.05,0.3) ), 0.05);
    d_neck += 0.003f*pa;
    d_neck *= 0.7f;

    if(d_neck < res.x) res = {d_neck,MAT_NECK_MASHROOM};

    return res;
}


vec2f lady_bug( vec3f p)
{

    p = worldToLadyBug(p);
    float dBody = sdEllipsoid( p, vec3f{}, vec3f{0.8f, 0.75f, 1.0f} );
    dBody = smax( dBody, -sdEllipsoid( p, vec3f{0.0f,-0.1f,0.0f}, vec3f{0.75f, 0.7f, 0.95f} ), 0.05f );
    dBody = smax( dBody, -sdEllipsoid( p, vec3f{0.0f,0.0f,0.8f}, vec3f{0.35f, 0.35f, 0.5f} ), 0.05f );
    dBody = smax( dBody, sdEllipsoid( p, vec3f{0.0f,1.7f,-0.1f}, vec3f(2.0f) ), 0.05f );
    dBody = smax( dBody, -abs(p.x)+0.005f, 0.02f + 0.1f*clamp(p.z*p.z*p.z*p.z,0.0f,1.0f));

    vec2f res =  {dBody/4.0f,MAT_LADY_BODY};


    // --------
    vec3f hc = vec3f(0.0,0.1,0.8);
    vec3f ph = rotateX(0.5)*(p-hc);
    float dHead = sdEllipsoid( ph, vec3f(0.0,0.0,0.0), vec3f(0.35, 0.25, 0.3) );
    dHead = smax( dHead, -sdEllipsoid( ph, vec3f(0.0,-0.95f,0.0), vec3f(1.0) ), 0.03 );
    dHead = min( dHead, sdEllipsoid( ph, vec3f(0.0,0.1,0.3), vec3f(0.15,0.08,0.15) ) );

    if( dHead < res.x ) res = vec2f( dHead, MAT_LADY_HEAD);


    res.x += 0.0007*sin(150.0*p.x)*sin(150.0*p.z)*sin(150.0*p.y); // iqiq
//
//    // -------------
//
    vec3f k1 = vec3f(0.42,-0.05f,0.92);
    vec3f k2 = vec3f(0.49,-0.2f,1.05);
    float dLegs = 10.0;

    float sx = sign(p.x);
    p.x = abs(p.x);
    for( int k=0; k<3; k++ )
    {
        vec3f q = p;
        q.y -= min(sx,0.0)*0.1;
        if( k==0) q += vec3f( 0.0,0.11,0.0);
        if( k==1) q += vec3f(-0.3f,0.1,0.2);
        if( k==2) q += vec3f(-0.3f,0.1,0.6);

        vec2f se = sdLine( q, vec3f(0.3,0.1,0.8), k1 );
        se.x -= 0.015 + 0.15*se.y*se.y*(1.0-se.y);
        dLegs = min(dLegs,se.x);

        se = sdLine( q, k1, k2 );
        se.x -= 0.01 + 0.01*se.y;
        dLegs = min(dLegs,se.x);

        se = sdLine( q, k2, k2 + vec3f(0.1,0.0,0.1) );
        se.x -= 0.02 - 0.01*se.y;
        dLegs = min(dLegs,se.x);
    }

    if( dLegs<res.x ) res = vec2f(dLegs,MAT_LADY_LEGS);


    return res;
}

float mod(float x, float y){
    auto res = fmodf(x,y);
    if (res < 0) res += y;
    return res;
}

float pMod1(float p, float size) {
    float halfsize = size*0.5f;
    //float c = floor((p + halfsize)/size);
    p = mod(p + halfsize, size) - halfsize;
    return p;
}



float vmax(vec3f v) {
    return max(max(v.x, v.y), v.z);
}

float vmax(vec2f v) {
    return max(v.x, v.y);
}

// Box: correct distance to corners
float fBox(vec3f p, vec3f b) {
    vec3f d = abs(p) - b;
    return length(max(d, vec3f(0))) + vmax(min(d, vec3f(0)));
}

float fCylinder(vec3f p, float r, float height) {
    float d = length(vec2f(p.x,p.z)) - r;
    d = max(d, abs(p.y) - height);
    return d;
}


float fBox2(vec2f p, vec2f b) {
    vec2f d = abs(p) - b;
    return length(max(d, vec2f(0))) + vmax(min(d, vec2f(0)));
}

float fOpIntersectionChamfer(float a, float b, float r) {
    return max(max(a, b), (a + r + b)*sqrtf(0.5));
}

// Difference can be built from Intersection or Union:
float fOpDifferenceChamfer (float a, float b, float r) {
    return fOpIntersectionChamfer(a, -b, r);
}

vec2f pR45(vec2f p) {
    p = (p + vec2f(p.y, -p.x))*sqrt(0.5);
    return p;
}

vec2f pR(vec2f p, float a) {
    p = cos(a)*p + sin(a)*vec2f(p.y, -p.x);
    return p;
}

float fOpDifferenceColumns(float a, float b, float r, float n) {
    a = -a;
    float m = min(a, b);
    //avoid the expensive computation where not needed (produces discontinuity though)
    if ((a < r) && (b < r)) {
        vec2f p = vec2f(a, b);
        float columnradius = r*sqrtf(2)/n/2.0f;
        columnradius = r*sqrtf(2)/((n-1)*2+sqrtf(2));

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

vec2f mod(vec2f x,vec2f y){
    auto res = vec2f{fmodf(x.x,y.x),fmodf(x.y,y.y)};
    if (res.x < 0) res.x += y.x;
    if (res.y < 0) res.y += y.y;
    return res;
}

vec2f floor(vec2f v){
    return {floor(v.x),floor(v.y)};
}

vec2f pModMirror2(vec2f p, vec2f size) {
    vec2f halfsize = size*0.5;
    vec2f c = floor((p + halfsize)/size);
    p = mod(p + halfsize, size) - halfsize;
    p *= mod(c,vec2f(2))*2.0f - vec2f(1);
    return p;
}

// Mirror at an axis-aligned plane which is at a specified distance <dist> from the origin.
float pMirror (float p, float dist) {
    float s = sign(p);
    p = abs(p)-dist;
    return p;
}

// Mirror in both dimensions and at the diagonal, yielding one eighth of the space.
// translate by dist before mirroring.
vec2f pMirrorOctant (vec2f p, vec2f dist) {
    vec2f s = vec2f{sign(p.x),sign(p.x)};
    p.x = pMirror(p.x, dist.x);
    p.y = pMirror(p.y, dist.y);
    if (p.y > p.x){
        auto t = p.x;
        p.x = p.y;
        p.y = p.x;
    }


    return p;
}

vec2f sceneSDF(vec3f pos) {
//
//    auto xz = pMirrorOctant({pos.x,pos.z},vec2f{50,40});
//    pos.x = xz.x;
//    pos.z = xz.y;

   auto  xz = pMirrorOctant({pos.x,pos.z},vec2f{30,40});
    pos.x = xz.x;
    pos.z = xz.y;

//    pos.z = -abs(pos.z) + 10;

    pos.x = -abs(pos.x) + 25;

    pos.z = pMod1(pos.z,20);

    float wall = fBox2(vec2f{pos.x,pos.y},vec2f{1,15});

    pos.z = abs(pos.z) - 3;
    pos.z = abs(pos.z) + 2;

    auto box = fBox(pos,vec3f{3,9,4});
    pos.y -= 9;
    auto cylinder = fCylinder(vec3f{pos.y,pos.x,pos.z},4,3);

    pos.y -= 6;
//    pos = pos * rotateX(0.7f);
//    pos = pos * rotateY(0.7f);
    auto xy = pR(vec2f{pos.x,pos.y},0.5f);
    pos.x = xy.x;
    pos.y = xy.y;
    pos.x -= 18;
    float roof = fBox2(vec2f{pos.x,pos.y},vec2f{20,0.5f});

    auto window = min(box,cylinder);

    auto building = fOpDifferenceColumns(wall,window,0.6f,3);
    building = min(roof,building);

    return vec2f{building,MAT_SILVER};
//    auto mush = mushroom(pos);
//    auto bug = lady_bug(pos);
//
//    if (bug.x < mush.x) return bug;
//    else return mush;


//    auto d_plane = udBox(pos, {10.0f,0.001f,10.0f});
//    auto d_cube = sdStrangerCube(((pos + vec3f{0.0f,-1.0f,1.5f}) * rotateZ(pif/4.0f) * rotateY(pif/4.0f))*2.0f)/2.0f;
//    if (d_plane < d_cube) return {d_plane, MAT_PLANE};
//    else return {d_cube, MAT_CUBE};

//    code used to test_img3.hdr
//    auto d_plane = udBox(pos, {10.0f,0.001f,10.0f});
//
//
//    auto d_torus = sdTorus((pos+ vec3f{0.0f,-0.75f,2.0f}) * rotateX(pif/4.0f), {0.35,0.1f});
//    d_torus = unionSDF(d_torus, sdTorus((pos+ vec3f{0.5f,-0.75f,2.0f}) * rotateX(pif/-4.0f), {0.35,0.1f}));
//    d_torus = unionSDF(d_torus, sdTorus((pos+ vec3f{-0.5f,-0.75f,2.0f}) * rotateX(pif/-4.0f), {0.35,0.1f}));
//    d_torus = unionSDF(d_torus, sdTorus((pos+ vec3f{-1.0f,-0.75f,2.0f}) * rotateX(pif/4.0f), {0.35,0.1f}));
//    d_torus = unionSDF(d_torus, sdTorus((pos+ vec3f{1.0f,-0.75f,2.0f}) * rotateX(pif/4.0f), {0.35,0.1f}));
//    d_torus = unionSDF(d_torus,sdTorus((pos+ vec3f{1.5f,-0.75f,2.0}) * rotateX(pif/-4.0f), {0.35,0.1f}));
//    d_torus = unionSDF(d_torus,sdTorus((pos+ vec3f{-1.5f,-0.75f,2.0f}) * rotateX(pif/-4.0f), {0.35,0.1f}));
//
//    if (d_plane < d_torus) return {d_plane, MAT_PLANE};
//    else return {d_torus, MAT_CUBE};

//    code used to test_img2.hdr
//    auto d_plane = udBox(pos, {10.0f,0.001f,10.0f});
//
//    auto tpos = (pos + vec3f{-1.6f,-1.0f,1.0f}) * rotateZ(pif/6.0f) * rotateY(pif/4.0f);
//    auto d_cube = sdBox((pos + vec3f{1.6f,-1.0f,-3.0f}) * rotateZ(pif/6.0f) * rotateY(pif/4.0f),{0.5f,0.5f,0.5f});
//    auto d_sphere = sdSphere(pos + vec3f{0.0f,-1.0f,-1.0f},0.6f);
//    auto d_cube_sphere = differenceSDF(sdBox(tpos,{0.5f,0.5f,0.5f}),sdSphere(tpos,0.6f));
//    auto shapes = unionSDF(d_cube,unionSDF(d_sphere,d_cube_sphere));
//
//    if (d_plane < shapes) return {d_plane, MAT_PLANE};
//    else return {shapes, MAT_CUBE};

//    code used to test_img1.hdr
//    auto d_plane = udBox(pos, {10.0f,0.001f,10.0f});
//
//    auto rtpos = (pos + vec3f{0.0f,-1.0f,2.0f})  * rotateZ(pif/4.0f) * rotateY(pif/4.0f);
//    auto d_cube = sdStrangerCube(rtpos*2.0f) / 2.0f;
//
//    if (d_plane < d_cube) return {d_plane, MAT_PLANE};
//    else return {d_cube, MAT_CUBE};

}
