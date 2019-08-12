# FullEarth
FullEarth is an OpenGL 3D globe representation of earthquakes around the world.
It uses GEOJSON has sources and is compatible with USGS notification service.
The software is currently in his infancy, it delivers basic manipulation but still miss a large number of features.
## Few images 
<img src="https://cdn.eintel.net/Images/Software/fe-one.png"></img>

<img src="https://cdn.eintel.net/Images/Software/fe-two.png"></img>

<img src="https://cdn.eintel.net/Images/Software/fe-dutch.png"></img>

## Dependency 

| LIB | Version | Description|
|-----|---------|------------|
| FLTK | 1.3.4 | An open source GUI library working on all platform. Can be compiled has a static library, reducing the number of DLL for windows platforms. |
| OpenGL | 19.0.8 | The 3D base software.|
| CURL | 7.58.0 | Software to interact with web servers.|
| OpenSSL | 1.0.2-nss| Encryption Software. Used by Curl to communicate with USGS servers using HTTPS protocol. 
| GLU  | 9.0.0 | Extension for OpenGL |
| GLEW | 2.0 | OpenGL loaders added to the software to add compatibility with Window OS. Sadly, I was not hable to compile on Windows Platform... (I NEED HELP HERE)|

## Installation
Install the dependency for your OS, then the easy way is to use CMAKE.
For Debian based system its look like this:
`sudo apt install build-essential libcurl4-openssl-dev libglew-dev libfltk1.3-dev libglu1-mesa-dev;`

Then use cmake:
`cmake ./FullEarthDir; make;`

There is no install script for now. The binary will be produced in the root directory.

## Precompiled Version

Precompiled Versions:

|Platforme|Processor|Link|
|---------|---------|----|
| Linux   | x86_64  | <a href="https://cdn.eintel.net/Softwares/FullEarth-0.1.1-Linux.tar.gz"> FullEarth-0.1.1-Linux.tar.gz</a>
