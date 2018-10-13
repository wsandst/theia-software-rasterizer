#pragma once
#include "Eigen/Dense"

using namespace Eigen;

struct Primitive //Triangle polygon
{
	//Indexes in Vertices for the primitives
	Vector3i points;
	Vector3i normals;
	Vector3i UVcoords;
	Vector3i colors;
	Primitive(int p1, int p2, int p3)
	{
		points = Vector3i(p1, p2, p3);
		normals = Vector3i(0, 0, 0);
		UVcoords = Vector3i(0, 0, 0);
		colors = Vector3i(0, 0, 0);
	}
	Primitive(int p1, int p2, int p3, int uv1, int uv2, int uv3, int n1, int n2, int n3)
	{
		points = Vector3i(p1, p2, p3);
		normals = Vector3i(n1, n2, n3);
		UVcoords = Vector3i(uv1, uv2, uv3);
		colors = Vector3i(0, 0, 0);
	}
	Primitive() {};
};

