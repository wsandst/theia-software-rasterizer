#pragma once
#include "Fragments.h"
#include <vector>

class FragmentShaders
{
public:
	//Copies the interpolated color value into the output
	static std::vector<Fragment> simpleColorShader(std::vector<Fragment> fragments)
	{
		for (size_t i = 0; i < fragments.size(); i++)
		{
			fragments[i].outValue = fragments[i].color;
		}
		return fragments;
	}
private:
	FragmentShaders();
	~FragmentShaders();
};

