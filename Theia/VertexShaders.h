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

		//for (size_t i = 0; i < vertices.normals.cols(); i++) { cout << to_string(vertices.normals(0, i)) << "," << to_string(vertices.normals(1, i)) << "," << to_string(vertices.normals(2, i)) << "," << endl; }

		newVertices.normals = projectionMatrix.inverse().transpose() * vertices.normals;
		newVertices.normalizeNormals();

		//for (size_t i = 0; i < newVertices.normals.cols(); i++) { cout << to_string(newVertices.normals(0, i)) << "," << to_string(newVertices.normals(1, i)) << "," << to_string(newVertices.normals(2, i)) << "," << endl; }
		//Perspective divide
		for (size_t i = 0; i < newVertices.points.cols(); i++) //This is slow, optimize!
		{
			newVertices.points(0, i) /= newVertices.points(3, i);
			newVertices.points(1, i) /= newVertices.points(3, i);
			newVertices.points(2, i) /= newVertices.points(3, i);
			//newVertices.points(3, i) = 1;
		}
		return newVertices;
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
	VertexShaders() {};
	~VertexShaders() {};
};

