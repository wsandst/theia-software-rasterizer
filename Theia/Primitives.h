#pragma once
#include <Eigen\Dense>

using namespace Eigen;

//Triangle polygon used in all rendering. Mainly stored in an Object.
struct Face
{
	unsigned int vertexA;
	unsigned int vertexB;
	unsigned int vertexC;
	unsigned int normalA;
	unsigned int normalB;
	unsigned int normalC;
	unsigned int UV_A;
	unsigned int UV_B;
	unsigned int UV_C;
	Vector3f color;
	Face(int a, int b, int c, Vector3f color)
	{
		this->vertexA = a;
		this->vertexB = b;
		this->vertexC = c;
		this->color = color;
	}
	Face(int va, int vb, int vc, int vta, int vtb, int vtc, int na, int nb, int nc, Vector3f color)
	{
		this->vertexA = va;
		this->vertexB = vb;
		this->vertexC = vc;
		this->normalA = na;
		this->normalB = nb;
		this->normalC = nc;
		this->UV_A = vta;
		this->UV_B = vtb;
		this->UV_C = vtc;
		this->color = color;
	}
};


