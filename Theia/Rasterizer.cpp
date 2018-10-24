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

float Rasterizer::cross2D(float x0, float y0, float x1, float y1)
{
	return (x0 * y1 - x1 * y0);
}


float Rasterizer::lineSide2D(Vector2f p, Vector2f lineFrom, Vector2f lineTo)
{
	return cross2D(p[0] - lineFrom[0], p[1] - lineFrom[1], lineTo[0] - lineFrom[0], lineTo[1] - lineFrom[1]);
}

Vector2f Rasterizer::interpolateBCVec2(Vector3f &bc, float& w, vector<Vector2f>& value)
{
	return (bc[0] * value[0] + bc[1] * value[1] + bc[2] * value[2]) * w;
}

Vector3f Rasterizer::interpolateBCVec3(Vector3f &bc, float& w, vector<Vector3f>& value)
{
	return (bc[0] * value[0] + bc[1] * value[1] + bc[2] * value[2]) * w;
}

Vector4f Rasterizer::interpolateBCVec4(Vector3f &bc, float& w, vector<Vector4f>& value)
{
	return (bc[0] * value[0] + bc[1] * value[1] + bc[2] * value[2]) * w;
}

void Rasterizer::scanLine(vector<Fragment>* fragments, int y, Vector2f pa, Vector2f pb, Vector2f pc, Vector2f pd, Vertex v1, Vertex v2, Vertex v3, VertexScanAttrib *vDiv)
{
	// Thanks to current Y, we can compute the gradient to compute others values like
	// the starting X (sx) and ending X (ex) to draw between
	// if pa.Y == pb.Y or pc.Y == pd.Y, gradient is forced to 1
	float gradient1 = pa[1] != pb[1] ? (y - pa[1]) / (pb[1] - pa[1]) : 1;
	float gradient2 = pc[1] != pd[1] ? (y - pc[1]) / (pd[1] - pc[1]) : 1;

	int sx = interpolate(pa[0], pb[0], gradient1);
	int ex = interpolate(pc[0], pd[0], gradient2) + 1;

	Vector3f bc;

	Fragment frag;

	float z, w;
	// drawing a line from left (sx) to right (ex) 
	for (int x = sx; x < ex; x++)
	{
		bc = barycentric(v1.point, v2.point, v3.point, x, y);
		if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) continue; //Point is not inside triangle
		z = v1.point[2] * bc[0] + v2.point[2] * bc[1] + v3.point[2] * bc[2];

		//Perspective correct interpolation
		w = 1 / (bc[0] * vDiv->wInverse[0] + bc[1] * vDiv->wInverse[1] + bc[2] * vDiv->wInverse[2]);

		//UVCoord = (bc[0] * vDiv->uvDivided[0] + bc[1] * vDiv->uvDivided[1] + bc[2] * vDiv->uvDivided[2]) * w;
		//color = (bc[0] * vDiv->colorDivided[0] + bc[1] * vDiv->colorDivided[1] + bc[2] * vDiv->colorDivided[2]) * w;
		//oldNormal = (bc[0] * vDiv->normalDivided[0] + bc[1] * vDiv->normalDivided[1] + bc[2] * vDiv->normalDivided[2]) * w;
		//normal = Vector3f(oldNormal[0], oldNormal[1], oldNormal[2]);

		frag = Fragment(Vector3f(x, y, z),
			interpolateBCVec3(bc, w, vDiv->normalDivided),
			interpolateBCVec2(bc, w, vDiv->uvDivided),
			interpolateBCVec4(bc, w, vDiv->colorDivided));

		fragments->push_back(frag);
	}
}


float Rasterizer::interpolate(float min, float max, float gradient)
{
	return min + (max - min) * clamp(gradient, 0, 1);
}

void Rasterizer::createPolyFragments2(vector<Fragment>* fragments, Vertex v1, Vertex v2, Vertex v3, int viewWidth, int viewHeight)
{
	Vector2f p1(v1.point[0], v1.point[1]);
	Vector2f p2(v2.point[0], v2.point[1]);
	Vector2f p3(v3.point[0], v3.point[1]);

	if (round(p1[1]) == round(p2[1]) && round(p1[1]) == p3[1]) return;
	if (p1[1] > p2[1]) std::swap(p1, p2); //Bubblesort in-place
	if (p1[1] > p3[1]) std::swap(p1, p3);
	if (p2[1] > p3[1]) std::swap(p2, p3);

	VertexScanAttrib vAttrib = VertexScanAttrib(v1, v2, v3);

	int height = p3[1];

	if ((lineSide2D(p2, p1, p3) > 0))
	{
		for (int y = p1[1]; y <= height; y++)
		{
			if (y < p2[1])
			{
				scanLine(fragments, y, p1, p3, p1, p2, v1, v2, v3, &vAttrib);
			}
			else
			{
				scanLine(fragments, y, p1, p3, p2, p3, v1, v2, v3, &vAttrib);
			}
		}
	}
	else
	{
		for (int y = p1[1]; y <= height; y++)
		{
			if (y < p2[1])
			{
				scanLine(fragments, y, p1, p2, p1, p3, v1, v2, v3, &vAttrib);
			}
			else
			{
				scanLine(fragments, y, p2, p3, p1, p3, v1, v2, v3, &vAttrib);
			}
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
