#pragma once
#include <vector>
#include "Primitives.h"
#include "Eigen/Dense"
#include "TextureManager.h"

using namespace std;

struct Material
{
	//Records information from mtl file
	string name;
	bool isTextured = false;
	int illumType = 2; //Illumination type, use this to decide how to render the material
					   //Color information (0 to 1)
	Vector3f ambient;  //Ka
	Vector3f diffuse;  //Kd
	Vector3f specular; //Ks
	Vector3f emissive; //Ke

	float specularExp; //Ns (Usually 0 to 1000)

					   //Light pass-through
	Vector3f tFilter; //tF
	float transparency; //d, (0 to 1)
	float opticalDensity; //For refraction on passthrough

	TexturePtr ambientTexture;
	TexturePtr diffuseTexture;
	TexturePtr specularTexture;
};

class Object
{
public:
	vector<Face> faces;
	Matrix4Xf vertices;
	Matrix4Xf normals;
	Matrix2Xf UVCoords;

	Matrix4f localTransMatrix;
	Vector3f rotation;
	Vector3f translation;
	float scale = 1;

	bool isConvex = true;
	bool isTextured = false;

	//Texture and light info from mtl file
	Material material;

	int faceCount = 0;
	int vertexCount = 0;
	int normalCount = 0;
	int UVCount = 0;
	void normalize();
	void updateTransMatrix();
	Object();
	~Object();
};

