#pragma once
#include "Fragments.h"
#include <vector>
#include "TextureManager.h"
#include <algorithm>
#include "Eigen/Dense"
#include <math.h>

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
	static void textureShader(std::vector<Fragment> &fragments, TextureWrapper& texture)
	{
		for (size_t i = 0; i < fragments.size(); i++) //Separate function maybe?
		{
			Vector4f pixel = texture.getPixel(fragments[i].UVcoord[0], fragments[i].UVcoord[1]);
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
			Vector4f tPixel = getPixel(newX, lineoffset, xOffset, texture);
			fragments[i].outValue = (tPixel + fragments[i].color) / 2;
		}


	}
	static void colorAndNormalShader(std::vector<Fragment> &fragments, Vector3f lightDirection, Vector4f lightColor)
	{
		for (size_t i = 0; i < fragments.size(); i++)
		{
			float cosTheta = clamp(fragments[i].normal.dot(lightDirection), 0.1, 1);

			fragments[i].outValue = ((fragments[i].color + lightColor)/2) * cosTheta;
		}
	}
	static void textureLightShader(std::vector<Fragment> &fragments, TexturePtr texturePtr, Vector3f lightDirection, Vector4f lightColor)
	{
		int tWidth = texturePtr->clip_rect.w;
		int tHeight = texturePtr->clip_rect.h;
		int pitch = texturePtr->pitch;
		int xOffset = pitch / tWidth;
		unsigned char* texture = (unsigned char*)texturePtr->pixels;
		for (size_t i = 0; i < fragments.size(); i++)
		{
			float cosTheta = clamp(fragments[i].normal.dot(lightDirection), 0.03, 1);

			int lineoffset = int(fragments[i].UVcoord[1] * tHeight) * pitch;
			int newX = int(fragments[i].UVcoord[0] * tWidth) * xOffset;
			Vector4f pixel = getPixel(newX, lineoffset, xOffset, texture);
			fragments[i].outValue = pixel * cosTheta;
		}
	}
private:
	static float clamp(float x, float lower, float upper)
	{
		return min(upper, max(x, lower));
	}
	static Vector4f getPixel(int x, int lineoffset, int xOffset, unsigned char* texture)
	{
		//Might want to add something here later for grayscale 
		if (xOffset == 3) //No transparency value
		{
			return Vector4f(texture[lineoffset + x] / 255.0f, texture[lineoffset + x + 1] / 255.0f, texture[lineoffset + x + 2] / 255.0f, 1);
		}
		else //xOffset == 4
		{
			return Vector4f(texture[lineoffset + x] / 255.0f, texture[lineoffset + x + 1] / 255.0f, texture[lineoffset + x + 2] / 255.0f, texture[lineoffset + x + 3] / 255.0f);
		}
	}
	FragmentShaders() {};
	~FragmentShaders() {};
};

