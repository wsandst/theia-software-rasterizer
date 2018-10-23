### Theia - A Software Renderer written in C++ using Eigen and SDL  
## Description:  
Theia is a Software Renderer which supports Vertex and Fragment shaders and several passes. The program is supposed to be similiar to OpenGL, but due to optimization reasons the data structures vary a bit, as there is no GSL and everything needs to run on the CPU.  

## Dependencies:
Eigen, for fast Matrix and Vector operations.  
SDL, for displaying pixels on the screen and various input  
SDL_Image, for loading image files from the disk into SDL_Surface objects

## How to run:
First of all, you shouldn't be running this.  
If you really want to run it, you will need to download this Resource folder:  
https://drive.google.com/open?id=1WkT3noQEMB-yukWFBqtg2czfHa0dNR5v  
and place it next to the exe file (or in the Visual Studio project folder if you are running straight from Visual Studios)

## Pictures of the program  
# Textured Diablo model  
![Diablo model](https://i.imgur.com/rl4Vvtu.png)  

# Transparency  
![Transparency](https://i.imgur.com/WsuVH9m.png)

# Textured triangle with color interpolation  
![Triangle](https://i.imgur.com/od3a4C2.png)
