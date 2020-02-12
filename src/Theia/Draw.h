#pragma once
#include "Fragments.h"
#include "Framebuffer.h"'
#include <vector>
#include <iostream>

using namespace std;

class Draw
{
public:
	static void drawFragments(Framebuffer& framebuffer, std::vector<Fragment>& fragments);
	static Vector4f blend(Vector4f soureColor, Vector4f newColor);
	static void drawWireframe(unsigned char *frameBuffer); //Draws a wireframe from a Vertices object
	static void drawLine(unsigned char *frameBuffer);
private:
	Draw();
	~Draw();
};

