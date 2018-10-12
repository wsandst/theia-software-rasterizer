#pragma once
#include "Eigen/Dense"

using namespace Eigen;

class Camera
{
private:
	Vector3f rotation = Vector3f(0, 0, 0);
	Vector3f translation = Vector3f(0, 0, 0);
	Affine3f createRotationMatrix(Vector3f r);

public:
	float scale = 1;

	Matrix4f cameraViewMatrix;
	Matrix4f transformationMatrix;
	Matrix4f projectionMatrix;
	Matrix4f viewPortMatrix;
	void calculateTransformMatrix();
	void calculateCameraViewMatrix();
	void setTranslation(Vector3f translation);
	void setRotation(Vector3f rotation);
	void setScale(float scale);
	void setViewport(int width, int height);

	void move(float x, float y, float z);
	void rotate(float xr, float yr, float zr);
	void changeScale(float x);

	Camera();
	~Camera();
};

