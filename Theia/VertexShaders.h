#pragma once
#include "Vertices.h"
#include "Eigen/Dense"

using namespace Eigen;

class VertexShaders //Static class
{
public:
	static Vertices projectionShader(Vertices vertices, Camera& camera, Matrix4f modelMatrix)
	{
		Vertices newVertices(vertices);
		Matrix4f finalMatrix = camera.projectionMatrix * camera.viewMatrix * modelMatrix;

		newVertices.points = finalMatrix * vertices.points;

		//for (size_t i = 0; i < vertices.normals.cols(); i++) { cout << to_string(vertices.normals(0, i)) << "," << to_string(vertices.normals(1, i)) << "," << to_string(vertices.normals(2, i)) << "," << endl; }

		newVertices.normals = finalMatrix.inverse().transpose() * vertices.normals;
		newVertices.surfaceNormals = finalMatrix.inverse().transpose() * vertices.surfaceNormals;
		//newVertices.normalizeNormals();
		//newVertices.normalizeSurfaceNormals();

		//for (size_t i = 0; i < newVertices.normals.cols(); i++) { cout << to_string(newVertices.normals(0, i)) << "," << to_string(newVertices.normals(1, i)) << "," << to_string(newVertices.normals(2, i)) << "," << endl; }
		//Perspective divide

		//newVertices.points = camera.viewPortMatrix * newVertices.points;

		for (size_t i = 0; i < newVertices.points.cols(); i++) //This is slow, optimize!
		{
			newVertices.wVector[i] = 1;
			newVertices.points(0, i) /= newVertices.points(3, i);
			newVertices.points(1, i) /= newVertices.points(3, i);
			newVertices.points(2, i) /= newVertices.points(3, i);
			newVertices.points(3, i) = 1;
		}

		newVertices.points = camera.viewPortMatrix * newVertices.points;

		return newVertices;
	}
	static Vertices transformShader(Vertices vertices, Matrix4f transformMatrix)
	{
		Vertices newVertices(vertices);
		newVertices.points = transformMatrix * vertices.points;
		newVertices.normals = transformMatrix * vertices.normals;
		newVertices.surfaceNormals = transformMatrix * vertices.surfaceNormals;
		newVertices.normalizeNormals();
		return newVertices;
	}
private:
	VertexShaders() {};
	~VertexShaders() {};
};

