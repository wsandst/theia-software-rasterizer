#pragma once
#include <vector>
#include "Fragments.h"
#include "Vertices.h"

class Rasterizer
{
public:
	static std::vector<Fragment> createFragments(Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight); //Creates fragments from vertex
private:
	static Vector4i findBoundingBox(Vector2i a, Vector2i b, Vector2i c, int width, int height);
	static Vector3f barycentric(Vector4f a, Vector4f b, Vector4f c, int x, int y);
	Rasterizer();
	~Rasterizer();
};

