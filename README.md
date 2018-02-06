# Signed distance field project

SDF project based on Yocto/GL Library.




## Images
These images are rendered using raymarcher and the scenes are distance functions.
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
