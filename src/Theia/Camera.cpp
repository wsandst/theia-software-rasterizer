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

void Camera::calculateViewMatrix()
{
	Affine3f r = createRotationMatrix(rotation);
	Affine3f t = Affine3f(Translation3f(translation));
	Affine3f s = Affine3f(Scaling(scale));

	viewMatrix = (t*s*r).matrix();
}

void Camera::calculateProjectionMatrix()
{
	// General form of the Projection Matrix
	//
	// uh = Cot( fov/2 ) == 1/Tan(fov/2)
	// uw / uh = 1/aspect
	// 
	//   uw         0       0       0
	//    0        uh       0       0
	//    0         0      f/(f-n)  1
	//    0         0    -fn/(f-n)  0
	//

	const float tanHalfFov = tanh(90 * 0.5f * 3.14f / 180.0f);
	float s1 = 1.0f / (tanHalfFov * 1);
	float s2 = s1 / ((float)height / width);
	float zFar = 100;
	float zNear = 0.1;
	float zRange = zFar - zNear;
	projectionMatrix
		<< s1, 0, 0, 0,
		0, s2, 0, 0,
		0, 0, -zFar / zRange, -zNear * zFar / zRange,
		0, 0, -1, 0;
}

void Camera::updateMatrices()
{
	calculateProjectionMatrix();
	calculateViewMatrix();
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
	this->width = width;
	this->height = height;
	viewPortMatrix
		<< width / 2.f, 0, 0, width / 2.f,
		0, height / 2.f, 0, height / 2.f,
		0, 0, 1 / 2.f, 1 / 2.f,
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
