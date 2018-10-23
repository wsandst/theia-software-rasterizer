#pragma once
#include <vector>
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;

struct Framebuffer
{
	std::vector<float> colorBuffer;
	std::vector<float> depthBuffer;
	std::vector<int> stencilBuffer;

	std::vector<unsigned char> outputBuffer;

	std::vector<float> zeroColorBuffer;

	int width;
	int height;
	Framebuffer(int width, int height)
	{
		this->width = width;
		this->height = height;
		colorBuffer = std::vector<float>(width*height*4, 0);
		outputBuffer = std::vector<unsigned char>(width*height * 4, 0);
		depthBuffer = std::vector<float>(width*height, 0);
		stencilBuffer = std::vector<int>(width*height, 0);

		zeroColorBuffer = std::vector<float>(width*height*4, 0);

		for (size_t i = 0; i < width*height * 4; i += 4)
		{
			zeroColorBuffer[i + 0] = 0;
			zeroColorBuffer[i + 1] = 0;
			zeroColorBuffer[i + 2] = 0;
			zeroColorBuffer[i + 3] = 1;
		}
	};
	void clear()
	{
		//Clear color buffer
		colorBuffer = zeroColorBuffer;
		//std::fill(&colorBuffer[0], &colorBuffer[0] + width * height * 4, 0.0);


		//Clear depth buffer
		std::fill(&depthBuffer[0], &depthBuffer[0] + width * height, 255);
		//Clear stencil buffer
		std::fill(&stencilBuffer[0], &stencilBuffer[0] + width * height, 255);
		//Clear output buffer
		std::fill(&outputBuffer[0], &outputBuffer[0] + width * height * 4, char(0));
	}
	float getDepthValue(int x, int y)
	{
		return depthBuffer[y * width + x];
	}
	void setDepthValue(int x, int y, float z)
	{
		depthBuffer[y * width + x] = z;
	}
	Vector4f getColorValue(int x, int y)
	{
		return Vector4f(colorBuffer[y * 4 * width + x * 4 + 0],
		colorBuffer[y * 4 * width + x * 4 + 1],
		colorBuffer[y * 4 * width + x * 4 + 2],
		colorBuffer[y * 4 * width + x * 4 + 3]);
	}
	void setColorValue(int x, int y, Vector4f color)
	{
		colorBuffer[y * 4 * width + x * 4 + 0] = color[0];
		colorBuffer[y * 4 * width + x * 4 + 1] = color[1];
		colorBuffer[y * 4 * width + x * 4 + 2] = color[2];
		colorBuffer[y * 4 * width + x * 4 + 3] = color[3];
	}
	void generateOutputBuffer()
	{
		for (size_t i = 0; i < colorBuffer.size(); i += 4)
		{
			outputBuffer[i + 0] = colorBuffer[i + 2] * 255;
			outputBuffer[i + 1] = colorBuffer[i + 1] * 255;
			outputBuffer[i + 2] = colorBuffer[i + 0] * 255;
			outputBuffer[i + 3] = colorBuffer[i + 3] * 255;
		}
	}
	Framebuffer() {};
};

