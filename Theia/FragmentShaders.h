#pragma once
#include "Fragments.h"
#include <vector>
#include "TextureManager.h"
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

class FragmentShaders
{
public:
	//Copies the interpolated color value into the output
	static void simpleColorShader(std::vector<Fragment> &fragments)
	{
		for (size_t i = 0; i < fragments.size(); i++)
		{
			fragments[i].outValue = fragments[i].color;
		}
	}
	static void textureShader(std::vector<Fragment> &fragments, TexturePtr texturePtr)
	{
		int tWidth = texturePtr->clip_rect.w;
		int tHeight = texturePtr->clip_rect.h;
		int pitch = texturePtr->pitch;
		int xOffset = pitch / tWidth;
		unsigned char* texture = (unsigned char*)texturePtr->pixels;
		for (size_t i = 0; i < fragments.size(); i++)
		{
			int lineoffset = int(fragments[i].UVcoord[1] * tHeight) * pitch;
			int newX = int(fragments[i].UVcoord[0] * tWidth) * xOffset;
			Vector4f pixel = Vector4f(texture[lineoffset + newX] / 255.0f, texture[lineoffset + newX + 1] / 255.0f, texture[lineoffset + newX + 2] / 255.0f, 1);
			fragments[i].outValue = pixel;
		}
	}
	static void textureAndColorShader(std::vector<Fragment> &fragments, TexturePtr texturePtr)
	{
		int tWidth = texturePtr->clip_rect.w;
		int tHeight = texturePtr->clip_rect.h;
		int pitch = texturePtr->pitch;
		int xOffset = pitch / tWidth;
		unsigned char* texture = (unsigned char*)texturePtr->pixels;
		for (size_t i = 0; i < fragments.size(); i++)
		{
			int lineoffset = int(fragments[i].UVcoord[1] * tHeight) * pitch;
			int newX = int(fragments[i].UVcoord[0] * tWidth) * xOffset;
			Vector4f tPixel = Vector4f(texture[lineoffset + newX] / 255.0f, texture[lineoffset + newX + 1] / 255.0f, texture[lineoffset + newX + 2] / 255.0f, 1);
			fragments[i].outValue = (tPixel + fragments[i].color) / 2;
		}


	}
	static void colorAndNormalShader(std::vector<Fragment> &fragments, Vector3f light)
	{
		for (size_t i = 0; i < fragments.size(); i++)
		{
			float cosTheta = clamp(fragments[i].normal.dot(light), 0.1, 1);

			fragments[i].outValue = fragments[i].color *cosTheta;
		}
	}
private:
	static float clamp(float x, float lower, float upper)
	{
		return min(upper, max(x, lower));
	}
	FragmentShaders() {};
	~FragmentShaders() {};
};

