## Theia - A Software Rasterizer written in C++ using SDL2 and Eigen3 

Theia is a Software Rasterizer written in C++. The aim of the project was to understand the underlying shader graphics pipeline performed by APIs such as OpenGL.

## Dependencies:
Eigen3, for fast Matrix and Vector operations.  
SDL2, for displaying pixels on the screen and various input  
SDL2_Image, for loading image files from the disk into SDL_Surface objects

## How to run:
1. Clone the repo  
2. Compile the program using Visual Studios 2019
3. Place the requisite DLL files next to the compiled exe file. These are found under /lib/sdl2/lib/ and /lib/sdl2_image-2.0.5/lib/x64/.  

## Images showcasing the engine  
## Textured Diablo model  
![Diablo model](https://i.imgur.com/rl4Vvtu.png)  

## Transparency  
![Transparency](https://i.imgur.com/WsuVH9m.png)

## Textured triangle with color interpolation  
![Triangle](https://i.imgur.com/od3a4C2.png)
