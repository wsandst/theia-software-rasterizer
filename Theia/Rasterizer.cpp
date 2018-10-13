#include "stdafx.h"
#include "Rasterizer.h"

std::vector<Fragment> Rasterizer::createFragments(Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	
	//Generates a fragment polygon using homogenous coordinates.
	//In the future this will use recursive polygon clipping and bresenhams line algorithm
	//Should I be doing culling in here? I'm not sure? What if I want to move around the fragments like some crazy person?

	std::vector<Fragment> fragments = std::vector<Fragment>();
	//Do some resize magic here

	Vector4i bBox = findBoundingBox(Vector2i(ceil(v1.point[0]), ceil(v1.point[1])), 
		Vector2i(ceil(v2.point[0]), ceil(v2.point[1])),
		Vector2i(ceil(v3.point[0]), ceil(v3.point[1])),
		viewWidth, viewHeight);

	if (bBox[2] == 0 || bBox[3] == 0) return std::vector<Fragment>(); //No polygon visible/ nothing to draw
	float z, w;
	for (int y = bBox[1]; y < bBox[3]; y++)
	{
		for (int x = bBox[0]; x < bBox[2]; x++)
		{

			//Calculate barycentric coordinates
			Vector3f bc = barycentric(v1.point, v2.point, v3.point, x, y);
			if (bc[0]<0 || bc[1]<0 || bc[2]<0) continue; //Point is not inside triangle

			//Depth
			float z = v1.point[2] * bc[0] + v2.point[2] * bc[1] + v3.point[2] * bc[2];
			Vector3f point = Vector3f((float)x, (float)y, z);

			//Perspective correct interpolation
			w = 1 / (bc[0] * (1 / v1.point[3]) + bc[1] * (1 / v2.point[3]) + bc[2] * (1 / v3.point[3]));
			Vector2f UVCoord = (bc[0] * (v1.UVCoord / v1.point[3]) + bc[1] * (v2.UVCoord / v2.point[3]) + bc[2] * (v3.UVCoord / v3.point[3])) * w;
			Vector4f color = (bc[0] * (v1.color / v1.point[3]) + bc[1] * (v2.color / v2.point[3]) + bc[2] * (v3.color / v3.point[3])) * w;

			Fragment frag = Fragment(point, UVCoord, color);
			fragments.push_back(frag);
		}
	}
	return fragments;
}

Vector4i Rasterizer::findBoundingBox(Vector2i a, Vector2i b, Vector2i c, int width, int height)
{
	int xMin, yMin, xMax, yMax;
	xMin = min(a[0], b[0]);
	xMin = max(0, min(xMin, c[0]));

	yMin = min(a[1], b[1]);
	yMin = max(0, min(yMin, c[1]));

	xMax = max(a[0], b[0]);
	xMax = min(width, max(xMax, c[0]));

	yMax = max(a[1], b[1]);
	yMax = min(height, max(yMax, c[1]));
	return Vector4i(xMin, yMin, xMax, yMax);
}

Vector3f Rasterizer::barycentric(Vector4f a, Vector4f b, Vector4f c, int x, int y)
{
	Vector3f crossA(c[0] - a[0], b[0] - a[0], a[0] - x);
	Vector3f crossB(c[1] - a[1], b[1] - a[1], a[1] - y);
	Vector3f u = crossA.cross(crossB);
	if (abs(u[2])<1) return Vector3f(-1, 1, 1); //Degenerate triangle, return something bad
	return Vector3f(1.f - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]);
}

Rasterizer::Rasterizer()
{
}


Rasterizer::~Rasterizer()
{
}
