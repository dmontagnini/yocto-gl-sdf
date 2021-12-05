# Signed Distance Field

SDF project based on Yocto/GL Library.

Signed Distance Field is a method used to represent scenes in which each shape is defined by an implicit function. 
The scene is a function itself and combines shapes through union, intersection and difference operations. 
Using this method, you can create complex scenes in procedural way with few code lines. 

## Code structure

- **raymarcher.cpp**:
it contains the application code through which it is possible to render the image defined in scene_sdf.cpp.
- **scene_sdf.cpp**:
it contains the standard functions that must be completed to define the scene. The code below is an example of how to complete these functions in order to create a scene containing a sphere of unitary radius.
  - **load_texture**:
if needed, this function must be implemented to load textures.
In our example we do not use textures, so its body is empty.
  ``` shell
  void load_textures(){}
  ```
  - **make_camera**:
it defines camera position and its features.
  ``` shell
  camera make_camera() {
    vec3f ro = {0.0f,0.0f,5.0f}; // camera position
    vec3f ta = vec3f(0.0f,0.0f,0.0f); // point where the camera is looking at
    auto cam = camera();
    cam.frame = make_frame(ro,ta); // this is a function that returns the corresponding frame
    cam.yfov = 0.26f * 2.0f; // camera field of view
    return cam; 
  }
  ```
  - **make_lights**:
this function defines lights position and intensity.
  ``` shell
  std::vector<light_sdf*> make_lights(){
    auto lights = std::vector<light_sdf*>();

    auto l = new light_sdf();
    l->pos = vec3f{0.0f,6.0f,10.0f}; // light position
    l->ke = vec3f{100.0f, 100.0f, 100.0f}; // light intensity
    lights.push_back(l);
    
    return lights;
  }
  ```
  - **materials**:
this function defines material features for each shape in the scene.
  ``` shell
  ...
  #define MAT_SPHERE 0.0f // this is the sphere ID and it is used
                          // to associate a material to this shape
  ...
  
  material materials(float id, vec3f pos){

    auto mat = material();
    if(id == MAT_SPHERE){
        mat.kd = vec3f{0.8f,0.8f,0.8f}; // sphere color
    }
    return mat;
  }
  ```
  - **fScene**:
this function defines the scene.
Given the coordinates of a point, it returns the shortest distance between that point and some shape and also that shape ID.
  ``` shell
  vec2f fScene(vec3f pos){
    auto d_sphere = fSphere(pos, 1.0f); // this function is defined in func_sdf.cpp
    return {d_sphere, MAT_SPHERE};
  }
  ```
  
  This is the image obtained with the scene defined above:
  ``` shell
  ./../bin/yraymarcher -r 360 -s 3 -o "../images/sphere.hdr"
  ```
  ![Image](images/sphere.png)
  
- **math_sdf.cpp**:
it contains some basic functions to manipulate vectors, matrices and other useful math operations.
- **func_sdf.cpp**:
it includes some basic distance functions.

## Images
``` shell
./../bin/yraymarcher -s 3 -o "../images/out_menger_sponge.hdr"
```
The code for this scene is in "menger_sponge.txt".
![Image](images/out_menger_sponge.png)

``` shell
./../bin/yraymarcher -s 3 -o "../images/out_struct.hdr"
```
The code for this scene is in "struct.txt".
![Image](images/out_struct.png)

## Getting Started

### Build
How to build:
``` shell
mkdir build; cd build; cmake -A Win32 ..; cmake --build .
```

### Run
To run application:
``` shell
./../bin/yraymarcher
```

### Parameters
- **-r** or **--resolution**: to specify vertical resolution, default 720
- **-s** or **--sample**: to specify per-pixel samples, default 1
- **-a** or **--ambient**: to specify ambient color, default 0.1f
- **-o** or **--output**: to specify output image, default "../images/out.hdr"

For example:
``` shell
./../bin/yraymarcher -r 1024 -s 3 -a 0.2f -o "../images/yourout.hdr"
```

## Authors
* [Francesca Romana Mattei](https://github.com/francescaromana)
* [Dario Montagnini](https://github.com/dariomontagnini)
* [Marzia Riso](https://github.com/MarziaRiso)
