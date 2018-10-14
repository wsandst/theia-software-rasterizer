#pragma once
#include <vector>
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;

struct Framebuffer
{
	std::vector<unsigned char> colorBuffer;
	std::vector<float> depthBuffer;
	std::vector<int> stencilBuffer;
	int width;
	int height;
	Framebuffer(int width, int height)
	{
		this->width = width;
		this->height = height;
		colorBuffer = std::vector<unsigned char>(width*height*4, 0);
		depthBuffer = std::vector<float>(width*height, 0);
		stencilBuffer = std::vector<int>(width*height, 0);
	};
	void clear()
	{
		//Clear color buffer
		std::fill(&colorBuffer[0], &colorBuffer[0] + width * height * 4, char(0));
		//Clear depth buffer
		std::fill(&depthBuffer[0], &depthBuffer[0] + width * height, 255);
		//Clear stencil buffer
		std::fill(&stencilBuffer[0], &stencilBuffer[0] + width * height, 255);
	}
	float getDepthValue(int x, int y)
	{
		return depthBuffer[y * width + x];
	}
	void setDepthValue(int x, int y, float z)
	{
		depthBuffer[y * width + x] = z;
	}
	int getColorValue(int x, int y)
	{
		return 0;
	}
	void setColorValue(int x, int y, Vector4f color)
	{
		colorBuffer[y * 4 * width + x * 4 + 0] = color[2] * 255;
		colorBuffer[y * 4 * width + x * 4 + 1] = color[1] * 255;
		colorBuffer[y * 4 * width + x * 4 + 2] = color[0] * 255;
		colorBuffer[y * 4 * width + x * 4 + 3] = color[3]*255;
	}
	Framebuffer() {};
};

