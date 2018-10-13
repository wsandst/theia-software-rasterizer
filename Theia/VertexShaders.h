#pragma once
#include "Vertices.h"
#include "Eigen/Dense"

using namespace Eigen;

class VertexShaders //Static class
{
public:
	static Vertices perspectiveShader(Vertices vertices, Matrix4f projectionMatrix)
	{
		Vertices newVertices(vertices);
		newVertices.points = projectionMatrix * vertices.points;
		newVertices.normals = projectionMatrix.inverse().transpose() * vertices.normals;
		newVertices.normalizeNormals();
		//Perspective divide
		for (size_t i = 0; i < newVertices.points.cols(); i++) //This is slow, optimize!
		{
			newVertices.points(0, i) /= newVertices.points(3, i);
			newVertices.points(1, i) /= newVertices.points(3, i);
			newVertices.points(2, i) /= newVertices.points(3, i);
			newVertices.points(3, i) = 1;
		}
		return newVertices;
		return vertices;
	}
	static Vertices transformShader(Vertices vertices, Matrix4f transformMatrix)
	{
		Vertices newVertices(vertices);
		newVertices.points = transformMatrix * vertices.points;
		newVertices.normals = transformMatrix * vertices.normals;
		newVertices.normalizeNormals();
		return newVertices;
	}
private:
	VertexShaders();
	~VertexShaders();
};

