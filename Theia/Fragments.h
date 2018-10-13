#pragma once
#include "Eigen/Dense"
#include <vector>

using namespace Eigen;

struct Fragment
{
	Vector3f point;
	Vector2f UVcoord;
	Vector4f color;

	Vector4f outValue;
	int depth;

	Fragment(Vector3f point, Vector2f UVcoord, Vector4f color)
	{
		this->point = point;
		this->UVcoord = UVcoord;
		this->color = color;
	}
};

