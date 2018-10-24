#pragma once
#include <vector>
#include "Fragments.h"
#include "Vertices.h"
#include <iostream>
#include <algorithm>

struct VertexScanAttrib //Precomputes things for speed optimization. Struct just here for easy passing around
{
	Vector3f wInverse;
	vector<Vector2f> uvDivided;
	vector<Vector4f> colorDivided;
	vector<Vector3f> normalDivided;
	VertexScanAttrib(Vertex v1, Vertex v2, Vertex v3)
	{
		this->wInverse = Vector3f(1 / v1.point[3], 1 / v2.point[3], 1 / v3.point[3]);
		this->uvDivided = vector<Vector2f>{ v1.UVCoord / v1.point[3], v2.UVCoord / v2.point[3], v3.UVCoord / v3.point[3] };
		this->colorDivided = vector<Vector4f>{ v1.color / v1.point[3], v2.color / v2.point[3], v3.color / v3.point[3] };
		this->normalDivided = vector<Vector3f>{ v1.normal.head<3>() / v1.point[3], v2.normal.head<3>() / v2.point[3], v3.normal.head<3>() / v3.point[3] };
	}
};

class Rasterizer
{
public:
	static void createPolyFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight); //Creates fragments from vertex
	static void createPolyFragments2(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createLineFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
	static void createDebugFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight);
private:
	static Vector4i findBoundingBox(Vector2i a, Vector2i b, Vector2i c, int width, int height);
	static Vector3f barycentric(Vector4f a, Vector4f b, Vector4f c, int x, int y);
	static void bresenham(vector<Fragment>* fragments, int x0, int y0, int x1, int y1);
	static float clamp(float x, float lower, float upper)
	{
		return min(upper, max(x, lower));
	}
	static float cross2D(float x0, float y0, float x1, float y1);
	static float lineSide2D(Vector2f p, Vector2f lineFrom, Vector2f lineTo);
	static Vector2f interpolateBCVec2(Vector3f& bc, float& w, vector<Vector2f>& value);
	static Vector3f interpolateBCVec3(Vector3f& bc, float& w, vector<Vector3f>& value);
	static Vector4f interpolateBCVec4(Vector3f& bc, float& w, vector<Vector4f>& value);
	static void scanLine(vector<Fragment>* fragments, int y, Vector2f pa, Vector2f pb, Vector2f pc, Vector2f pd, Vertex v1, Vertex v2, Vertex v3, VertexScanAttrib *vDiv);
	static float interpolate(float min, float max, float gradient);
	Rasterizer();
	~Rasterizer();
};

