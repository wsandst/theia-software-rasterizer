#include "stdafx.h"
#include "Rasterizer.h"

void Rasterizer::createPolyFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	
	//Generates a fragment polygon using homogenous coordinates.
	//In the future this will use recursive polygon clipping and bresenhams line algorithm
	//Should I be doing culling in here? I'm not sure? What if I want to move around the fragments like some crazy person?

	Vector4i bBox = findBoundingBox(Vector2i(ceil(v1.point[0]), ceil(v1.point[1])), 
		Vector2i(ceil(v2.point[0]), ceil(v2.point[1])),
		Vector2i(ceil(v3.point[0]), ceil(v3.point[1])),
		viewWidth, viewHeight);

	if (bBox[2] == 0 || bBox[3] == 0) return; //No polygon visible/ nothing to draw
	float z, w;

	Vector3f wInverse = Vector3f(1 / v1.point[3], 1 / v2.point[3], 1 / v3.point[3]);
	vector<Vector2f> uvDivided = vector<Vector2f>{ v1.UVCoord / v1.point[3], v2.UVCoord / v2.point[3], v3.UVCoord / v3.point[3] };
	vector<Vector4f> colorDivided = vector<Vector4f>{ v1.color / v1.point[3], v2.color / v2.point[3], v3.color / v3.point[3] };
	vector<Vector4f> normalDivided = vector<Vector4f>{ v1.normal / v1.point[3], v2.normal / v2.point[3], v3.normal / v3.point[3] };
	Vector4f oldNormal;
	Vector3f bc, point, normal;
	Vector4f color;
	Vector2f UVCoord;

	for (int y = bBox[1]; y < bBox[3]; y++)
	{
		for (int x = bBox[0]; x < bBox[2]; x++)
		{

			//Calculate barycentric coordinates
			bc = barycentric(v1.point, v2.point, v3.point, x, y);
			if (bc[0]<0 || bc[1]<0 || bc[2]<0) continue; //Point is not inside triangle

			//Depth
			z = v1.point[2] * bc[0] + v2.point[2] * bc[1] + v3.point[2] * bc[2];
			point = Vector3f(x, y, z);

			//Perspective correct interpolation
			w = 1 / (bc[0] * wInverse[0] + bc[1] * wInverse[1] + bc[2] * wInverse[2]);

			UVCoord = (bc[0] * uvDivided[0] + bc[1] * uvDivided[1] + bc[2] * uvDivided[2]) * w;
			color = (bc[0] * colorDivided[0] + bc[1] * colorDivided[1] + bc[2] * colorDivided[2]) * w;
			oldNormal = (bc[0] * normalDivided[0] + bc[1] * normalDivided[1] + bc[2] * normalDivided[2]) * w;
			normal = Vector3f(oldNormal[0], oldNormal[1], oldNormal[2]);

			fragments->push_back(Fragment(point, normal, UVCoord, color));
		}
	}
}

void Rasterizer::createFragments3(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	//Uses bresenhams line algorithm twice
	Vector2f t0(v1.point[0], v1.point[1]);
	Vector2f t1(v2.point[0], v2.point[1]);
	Vector2f t2(v3.point[0], v3.point[1]);

	if (t0[1] == t1[1] && t0[1] == t2[1]) return;
	if (t0[1]>t1[1]) std::swap(t0, t1);
	if (t0[1]>t2[1]) std::swap(t0, t2);
	if (t1[1]>t2[1]) std::swap(t1, t2);
	int total_height = t2[1] - t0[1];

	float z, w;
	Vector3f wInverse = Vector3f(1 / v1.point[3], 1 / v2.point[3], 1 / v3.point[3]);
	vector<Vector2f> uvDivided = vector<Vector2f>{ v1.UVCoord / v1.point[3], v2.UVCoord / v2.point[3], v3.UVCoord / v3.point[3] };
	vector<Vector4f> colorDivided = vector<Vector4f>{ v1.color / v1.point[3], v2.color / v2.point[3], v3.color / v3.point[3] };
	Vector3f bc, point;
	Vector4f color;
	Vector2f UVCoord;

	for (int i = 0; i<total_height; i++) {
		bool second_half = i>t1[1] - t0[1] || t1[1] == t0[1];
		int segment_height = second_half ? t2[1] - t1[1] : t1[1] - t0[1];
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1[1] - t0[1] : 0)) / segment_height;
		Vector2i A = (t0 + (t2 - t0)*alpha).cast<int>();
		Vector2i B = second_half ? (t1 + (t2 - t1)*beta).cast<int>() : (t0 + (t1 - t0)*beta).cast<int>();
		if (A[0]>B[0]) std::swap(A, B);
		for (int j = A[0]; j <= B[0]; j++) {
			int x = j;
			int y = t0[1] + i;
			bc = barycentric(v1.point, v2.point, v3.point, x, y);
			if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue; //Point is not inside triangle
			z = v1.point[2] * bc[0] + v2.point[2] * bc[1] + v3.point[2] * bc[2];
			point = Vector3f(x, y, z);

			//Perspective correct interpolation
			w = 1 / (bc[0] * wInverse[0] + bc[1] * wInverse[1] + bc[2] * wInverse[2]);

			UVCoord = (bc[0] * uvDivided[0] + bc[1] * uvDivided[1] + bc[2] * uvDivided[2]) * w;
			color = (bc[0] * colorDivided[0] + bc[1] * colorDivided[1] + bc[2] * colorDivided[2]) * w;

			//fragments->push_back(Fragment(point, UVCoord, color));
		}
	}
}

void Rasterizer::createFragments2(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	//Uses bresenhams line algorithm twice
	if (v1.point[1] == v2.point[1] && v1.point[1] == v3.point[1]) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!) 
	if (v1.point[1]>v2.point[1]) std::swap(v1, v2);
	if (v1.point[1]>v3.point[1]) std::swap(v1, v3);
	if (v2.point[1]>v3.point[1]) std::swap(v2, v3);
	int total_height = v3.point[1] - v1.point[1];

	float z, w;
	Vector3f wInverse = Vector3f(1 / v1.point[3], 1 / v2.point[3], 1 / v3.point[3]);
	vector<Vector2f> uvDivided = vector<Vector2f>{ v1.UVCoord / v1.point[3], v2.UVCoord / v2.point[3], v3.UVCoord / v3.point[3] };
	vector<Vector4f> colorDivided = vector<Vector4f>{ v1.color / v1.point[3], v2.color / v2.point[3], v3.color / v3.point[3] };
	Vector3f bc, point;
	Vector4f color;
	Vector2f UVCoord;

	for (int i = 0; i < total_height; i++)
	{
		bool second_half = i > v2.point[1] - v1.point[1] || v2.point[1] == v1.point[1];
		int segment_height = second_half ? v3.point[1] - v2.point[1] : v2.point[1] - v1.point[1];
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? v2.point[1] - v1.point[1] : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		Vector2i A(v1.point[0] + (v3.point[0] - v1.point[0]) * alpha, v1.point[1] + (v3.point[1] - v1.point[1]));
		Vector2i B;
		if (second_half)
		{
			B = Vector2i(v2.point[0] + (v3.point[0] - v2.point[0]) * beta, v2.point[1] + (v3.point[1] - v2.point[1]) * beta);
		}
		else
		{
			B = Vector2i(v1.point[0] + (v2.point[0] - v1.point[0]) * beta, v1.point[1] + (v2.point[1] - v1.point[1]) * beta);
		}
		if (A[0] > B[0])
		{
			std::swap(A, B);
		}
		for (int j = A[0]; j <= B[0]; j++) {
			//Got the pixel here I think
			int x = j;
			int y = v1.point[0] + i;
			bc = barycentric(v1.point, v2.point, v3.point, x, y);
			if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue; //Point is not inside triangle

			//Depth
			z = v1.point[2] * bc[0] + v2.point[2] * bc[1] + v3.point[2] * bc[2];
			point = Vector3f(x, y, z);

			//Perspective correct interpolation
			w = 1 / (bc[0] * wInverse[0] + bc[1] * wInverse[1] + bc[2] * wInverse[2]);

			UVCoord = (bc[0] * uvDivided[0] + bc[1] * uvDivided[1] + bc[2] * uvDivided[2]) * w;
			color = (bc[0] * colorDivided[0] + bc[1] * colorDivided[1] + bc[2] * colorDivided[2]) * w;

			//fragments->push_back(Fragment(point, UVCoord, color));
		}
	}
}

void Rasterizer::createLineFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	bresenham(fragments, v1.point[0], v1.point[1], v2.point[0], v2.point[1]);
	bresenham(fragments, v1.point[0], v1.point[1], v3.point[0], v3.point[1]);
	bresenham(fragments, v2.point[0], v2.point[1], v3.point[0], v3.point[1]);
}

void Rasterizer::createDebugFragments(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	//Draws normals in Wireframe mode
	bresenham(fragments, v1.point[0], v1.point[1], v2.point[0], v2.point[1]);
	bresenham(fragments, v1.point[0], v1.point[1], v3.point[0], v3.point[1]);
	bresenham(fragments, v2.point[0], v2.point[1], v3.point[0], v3.point[1]);

	bresenham(fragments, v1.point[0], v1.point[1], v1.point[0]+ v1.normal[0]*50, v1.point[1] + v1.normal[1] * 50);
	bresenham(fragments, v2.point[0], v2.point[1], v2.point[0] + v2.normal[0] * 50, v2.point[1] + v2.normal[1] * 50);
	bresenham(fragments, v3.point[0], v3.point[1], v3.point[0] + v3.normal[0] * 50, v3.point[1] + v3.normal[1] * 50);
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

void Rasterizer::bresenham(vector<Fragment>* fragments, int x1, int y1, int x2, int y2)
{
	//cout << "Line from (" << to_string(x1) << "," << to_string(y1) << " to (" << to_string(x2) << "," << to_string(y2) << ")" << endl;
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		fragments->push_back(Fragment(Vector3f(x, y, 0), Vector4f(1, 1, 1, 1)));
		for (i = 0; x<xe; i++)
		{
			x = x + 1;
			if (px<0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			fragments->push_back(Fragment(Vector3f(x, y, 0), Vector4f(1, 1, 1, 1)));
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		fragments->push_back(Fragment(Vector3f(x, y, 0), Vector4f(1, 1, 1, 1)));
		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			fragments->push_back(Fragment(Vector3f(x, y, 0), Vector4f(1, 1, 1, 1)));
		}
	}
}

Rasterizer::Rasterizer()
{
}


Rasterizer::~Rasterizer()
{
}
