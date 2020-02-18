#include "Object.h"

void Object::normalize()
{
	//Normalizes object vertices between -1 to 1
	//Start out by finding largest absolute value
	double max = 0;
	for (size_t i = 0; i < vertices.getVertexCount(); i++)
	{
		if (abs(vertices.points(0, i)) > max)
			max = abs(vertices.points(0, i));
		if (abs(vertices.points(1, i)) > max)
			max = abs(vertices.points(1, i));
		if (abs(vertices.points(2, i)) > max)
			max = abs(vertices.points(2, i));
	}
	//Divide every coordinate by max
	for (size_t i = 0; i < vertices.getVertexCount(); i++)
	{
		vertices.points(0, i) /= max;
		vertices.points(1, i) /= max;
		vertices.points(2, i) /= max;
	}
}

void Object::updateModelMatrix()
{
	Affine3f rx =
		Affine3f(Eigen::AngleAxisf(rotation[0], Eigen::Vector3f(1, 0, 0)));
	Affine3f ry =
		Affine3f(Eigen::AngleAxisf(rotation[1], Eigen::Vector3f(0, 1, 0)));
	Affine3f rz =
		Affine3f(Eigen::AngleAxisf(rotation[2], Eigen::Vector3f(0, 0, 1)));
	Affine3f r = rz * ry * rx;

	Affine3f t = Affine3f(Translation3f(translation));
	Affine3f s = Affine3f(Scaling(scale));

	modelMatrix = (t*s*r).matrix();
}

Object::Object()
{
	rotation = Vector3f(0, 0, 0);
	translation = Vector3f(0, 0, 0);
	modelMatrix
		<< 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
}


Object::~Object()
{
}
