#pragma once
#include "Eigen/Dense"
#include <vector>
#include <algorithm>

using namespace Eigen;

//Due to performance optimizations, there are no isolated Vertex objects, but instead
//only Vertices, which is a group of vertex using matrices

struct Vertices
{
public:
	Matrix4Xf points;
	Matrix4Xf normals;
	Matrix2Xf UVCoords;
	Matrix3Xi colors;

	Vertices(Matrix4Xf points, Matrix4Xf normals, Matrix2Xf UVCoords, Matrix3Xi colors)
	{
		this->points = points;
		this->normals = normals;
		this->UVCoords = UVCoords;
		this->colors = colors;
	}
	Vertices(Matrix4Xf points)
	{
		this->points = points;
	}
	Vertices(Matrix4Xf points, Matrix3Xi colors)
	{
		this->points = points;
		this->colors = colors;
	}
	void combine(Vertices vertices2)
	{
		points << points, vertices2.points;
		normals << normals, vertices2.normals;
		UVCoords << UVCoords, vertices2.UVCoords;
		colors << colors, vertices2.colors;
	}
	void resize(int newSize)
	{
		points.conservativeResize(Eigen::NoChange, newSize);
		normals.conservativeResize(Eigen::NoChange, newSize);
		UVCoords.conservativeResize(Eigen::NoChange, newSize);
		colors.conservativeResize(Eigen::NoChange, newSize);
	}
	int getVertexCount()
	{
		return points.cols();
	}
	Vertices(int size)
	{
		points = Matrix4Xf();
		normals = Matrix4Xf();
		UVCoords = Matrix2Xf();
		colors = Matrix3Xi();
		points.resize(Eigen::NoChange, size);
		normals.resize(Eigen::NoChange, size);
		UVCoords.resize(Eigen::NoChange, size);
		colors.resize(Eigen::NoChange, size);
	};
	Vertices() {};
};

