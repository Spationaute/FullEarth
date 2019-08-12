# FullEarth
FullEarth is an OpenGL 3D globe representation of earthquakes around the world.
It uses GEOJSON has sources and is compatible with USGS notification service.
The software is currently in his infancy, it delivers basic manipulation but still miss a large number of features.
## Few images 
<img src="https://cdn.eintel.net/Images/Software/fe-one.png"></img>

<img src="https://cdn.eintel.net/Images/Software/fe-two.png"></img>

<img src="https://cdn.eintel.net/Images/Software/fe-dutch.png"></img>

## Dependency 

- **FLTK** : An open source GUI library working on all platform. 
         Can be compiled has a static library, reducing the number of DLL for windows platforms.

- **OpenGL**: The 3D base software.

- **CURL**: Software to interact with web servers.

- **GLEW**: OpenGL loaders added to the software to add compatibility with Window OS.
        Sadly, I was not hable to compile on Windows Platform... (I NEED HELP HERE)

## Installation
Install the dependency for your OS, then the easy way is to use CMAKE.
There is no install script for now. The binary will be produced in the root directory.

`cmake ./FullEarthDir; make;`  

## Precompiled Version

Watch closely, a precompiled version for Linux x86_64 will be added here.
