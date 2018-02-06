# Signed Distance Field

SDF project based on Yocto/GL Library.

Signed Distance Field is a method used to represent scenes in which each shape is defined by an implicit function. 
The scene is a function itself and combines shapes through union, intersection and difference operations. 
Using this method, you can create complex scenes in procedural way with few code lines. 

## Code structure

- **raymarcher.cpp**:
it contains the application code through which it is possible to render the image defined in scene_sdf.cpp.
- **scene_sdf.cpp**:
it contains the standard functions that must be completed to define the scene. The code below is an example of how to complete these functions in order to create a scene containing a sphere.
  - **load_texture**:
if needed, this function must be implemented to load textures.
In our example we do not use textures, so its body is empty.
  ``` shell
  void load_textures(){}
  ```
  - **make_camera**:
it defines camera position and its features.
  - **make_lights**:
this function defines lights position and intesity.
  - **materials**:
this function defines material features for each shape in the scene.
  - **fScene**:
this function defines the scene.
Given the coordinates of a point, it returns the shortest distance between that point and some surface and also the id shape that has be
- **math_sdf.cpp**:
it contains some basic fuction to manipulate vectors, matrices and other simple math operations.
- **func_sdf.cpp**:
in this file there are some basic distance functions.


## Images

![Image](images/out_menger_sponge.png)

## Getting Started

### Build
How to build:
``` shell
mkdir build; cd build; cmake ..; cmake --build .
```

### Run
To run application:
``` shell
./yraymarcher
```

### Parameters
- **-r** or **--resolution**: to specify vertical resolution, default 720
- **-s** or **--sample**: to specify per-pixel samples, default 1
- **-a** or **--ambient**: to specify ambient color, default 0.1f
- **-o** or **--output**: to specify output image, default "../images/out.hdr"

For example:
``` shell
./yraymarcher -r 1024 -s 3 -a 0.2f -o "../images/yourout.hdr"
```

## Authors
* **Mattei Francesca Romana** - ...
* **Montagnini Dario** - ...
* **Riso Marzia** - ..
