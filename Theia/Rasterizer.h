#pragma once
#include <vector>
#include "Fragments.h"
#include "Vertices.h"
#include <iostream>
#include <algorithm>

class Rasterizer
{
public:
	static void createFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight); //Creates fragments from vertex
	static void createFragments3(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createFragments2(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
private:
	static Vector4i findBoundingBox(Vector2i a, Vector2i b, Vector2i c, int width, int height);
	static Vector3f barycentric(Vector4f a, Vector4f b, Vector4f c, int x, int y);
	Rasterizer();
	~Rasterizer();
};

