#pragma once
#include "Eigen/Dense"
#include <vector>

using namespace Eigen;

struct Fragment
{
	Vector3f point;
	Vector2i UVcoord;
	Vector3f color;

	Vector3f outValue;
	int depth;
};

