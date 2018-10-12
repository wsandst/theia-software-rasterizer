#pragma once
#include <vector>
#include "Eigen/Dense"
#include "TextureManager.h"
#include "Vertices.h"
#include "Primitive.h"

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
	vector<Primitive> primitives;
	Vertices vertices;

	//Local transformation
	Matrix4f localTranslationMatrix;
	Vector3f rotation;
	Vector3f translation;
	float scale = 1;

	bool isConvex = true;

	//Texture and light info from mtl file
	Material material;

	void normalize();
	void updateTranslationMatrix();
	Object();
	~Object();
};

