#pragma once
#include "Eigen/Dense"
#include <vector>

using namespace Eigen;

struct Fragment
{
	Vector3f point;
	Vector3f normal;
	Vector2f UVcoord;
	Vector4f color;

	Vector4f outValue;
	int depth;

	Fragment(Vector3f point, Vector3f normal, Vector2f UVcoord, Vector4f color)
	{
		this->normal = normal;
		this->point = point;
		this->UVcoord = UVcoord;
		this->color = color;
	}
	Fragment(Vector3f point,  Vector4f color)
	{
		this->point = point;
		this->color = color;
	}
	Fragment(Vector3f point)
	{
		this->point = point;
		this->color = Vector4f(1, 1, 1, 1);
	}
	Fragment()
	{
	}
	bool operator < (const Fragment& frag) const //For sorting
	{
		return (point[2] > frag.point[2]);
	}
};

