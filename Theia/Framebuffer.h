#pragma once
#include <vector>

struct Framebuffer
{
	std::vector<unsigned char> colorBuffer;
	std::vector<int> depthBuffer;
	std::vector<int> stencilBuffer;
	int width;
	int height;
	Framebuffer(int width, int height)
	{
		this->width = width;
		this->height = height;
		colorBuffer = std::vector<unsigned char>(width*height*4, 0);
		depthBuffer = std::vector<int>(width*height, 0);
		stencilBuffer = std::vector<int>(width*height, 0);
	};
	Framebuffer() {};
};

