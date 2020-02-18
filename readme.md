## Theia - A Software Rasterizer written in C++ using SDL2 and Eigen3 

Theia is a Software Rasterizer written in C++. The aim of the project was to understand the underlying shader graphics pipeline performed by GPU APIs such as OpenGL.  

## Images showcasing the engine  
## Textured Diablo model  
![Diablo model](https://i.imgur.com/rl4Vvtu.png)  

## Transparency  
![Transparency](https://i.imgur.com/WsuVH9m.png)  

## Textured triangle with color interpolation  
![Triangle](https://i.imgur.com/od3a4C2.png)  
  
## Dependencies  
Eigen3, for fast Matrix and Vector operations. (included as git submodule)   
SDL2, for displaying pixels on the screen and various input. [Download link](https://www.libsdl.org/download-2.0.php)  
SDL2_Image, for loading image files from the disk. [Download link](https://www.libsdl.org/download-2.0.php)  
  
## Build instructions  
1. Install the required dependencies  
2. Clone the repo and the submodules ("git submodule update --init --recursive")  
3. Run cmake  
4. You can now compile/modify the project  