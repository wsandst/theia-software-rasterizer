#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

Affine3f Camera::createRotationMatrix(Vector3f r)
{
	Affine3f rx =
		Affine3f(Eigen::AngleAxisf(r[0], Eigen::Vector3f(1, 0, 0)));
	Affine3f ry =
		Affine3f(Eigen::AngleAxisf(r[1], Eigen::Vector3f(0, 1, 0)));
	Affine3f rz =
		Affine3f(Eigen::AngleAxisf(r[2], Eigen::Vector3f(0, 0, 1)));
	return rz * ry * rx;
}

void Camera::calculateTransformMatrix()
{
	Affine3f r = createRotationMatrix(rotation);
	Affine3f t = Affine3f(Translation3f(translation));
	Affine3f s = Affine3f(Scaling(scale));

	transformationMatrix = (t*s*r).matrix();
}

void Camera::calculateCameraViewMatrix()
{
	calculateTransformMatrix();
	cameraViewMatrix = viewPortMatrix * projectionMatrix * transformationMatrix;
}

void Camera::setTranslation(Vector3f translation)
{
	this->translation = translation;
}

void Camera::setRotation(Vector3f rotation)
{
	this->rotation = rotation;
}

void Camera::setScale(float scale)
{
	this->scale = scale;
}

void Camera::setViewport(int width, int height)
{
	viewPortMatrix
		<< width / 2.f, 0, 0, width / 2.f,
		0, height / 2.f, 0, height / 2.f,
		0, 0, 255 / 2.f, 255 / 2.f,
		0, 0, 0, 1;
}

void Camera::move(float x, float y, float z)
{
	translation[0] += x;
	translation[1] += y;
	translation[2] += z;
}

void Camera::rotate(float xr, float yr, float zr)
{
	rotation[0] += xr;
	rotation[1] += yr;
	rotation[2] += zr;
}

void Camera::changeScale(float x)
{
	scale *= scale;
}
