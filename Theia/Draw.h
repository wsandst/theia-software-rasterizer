#pragma once
#include "Fragments.h"
#include "Framebuffer.h"'
#include <vector>

class Draw
{
public:
	static Framebuffer drawFragments(Framebuffer framebuffer, std::vector<Fragment> fragments);
	static void drawWireframe(unsigned char *frameBuffer); //Draws a wireframe from a Vertices object
	static void drawLine(unsigned char *frameBuffer);
private:
	Draw();
	~Draw();
};

