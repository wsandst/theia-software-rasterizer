#pragma once
#include <vector>
#include "Fragments.h"
#include "Vertices.h"
#include <iostream>
#include <algorithm>

class Rasterizer
{
public:
	static void createPolyFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight); //Creates fragments from vertex
	static void createFragments3(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createFragments2(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createLineFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createDebugFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
private:
	static Vector4i findBoundingBox(Vector2i a, Vector2i b, Vector2i c, int width, int height);
	static Vector3f barycentric(Vector4f a, Vector4f b, Vector4f c, int x, int y);
	static void bresenham(vector<Fragment>* fragments, int x0, int y0, int x1, int y1);
	Rasterizer();
	~Rasterizer();
};

