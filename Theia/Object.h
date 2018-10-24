#pragma once
#include <vector>
#include "Eigen/Dense"
#include "TextureManager.h"
#include "Vertices.h"
#include "Primitive.h"

using namespace std;

enum WrapMode { WRAP_REPEAT, WRAP_MIRRORED_REPEAT, WRAP_CLAMP_TO_EDGE, WRAP_CLAMP_TO_BORDER };
enum InterpolationMode { NEAREST_NEIGHTBOUR, BILINEAR };

struct TextureWrapper
{
	TexturePtr texturePtr;
	unsigned char* texture;
	WrapMode wrapMode = WRAP_REPEAT;
	InterpolationMode interpolationMode = BILINEAR;

	int xOffset; //Bytecount per pixel
	int tWidth;
	int tHeight;
	int pitch;
	TextureWrapper(TexturePtr tPtr)
	{
		texturePtr = tPtr;
		tWidth = tPtr->clip_rect.w;
		tHeight = tPtr->clip_rect.h;
		pitch = tPtr->pitch;
		texture = (unsigned char*)texturePtr->pixels;
		xOffset = pitch / tWidth;
	}
	TextureWrapper() {};
	Vector4f getPixel(float x, float y)
	{
		switch (wrapMode)
		{
			case WRAP_REPEAT:
				x = fmod(x, tWidth);
				y = fmod(y, tHeight);
				break;
		}
		switch (interpolationMode)
		{
			case NEAREST_NEIGHTBOUR:
			{
				int lineoffset = int(y * tHeight) * pitch;
				int newX = int(x * tWidth) * xOffset;
				return retrievePixel(lineoffset, newX, xOffset);
				break;
			}
			case BILINEAR:
			{
				int y1 = y * tHeight;
				int y2 = min(y1 + 1, tHeight-1);
				int x1 = x * tWidth;
				int x2 = min(x1 + 1, tWidth-1);
				Vector4f p1, p2, p3, p4;

				p1 = retrievePixel(y1 * pitch, x1 * xOffset, xOffset);
				p2 = retrievePixel(y1 * pitch, x2 * xOffset, xOffset);
				p3 = retrievePixel(y2 * pitch, x1 * xOffset, xOffset);
				p4 = retrievePixel(y2 * pitch, x2 * xOffset, xOffset);

				x *= tWidth;
				y *= tHeight;

				float dx = x - x1;
				float dy = y - y1;

				return (p1 * (1 - dx) + p2 * dx) * (1 - dy) + (p3 * (1 - dx) + p4 * dx) * dy;
				break;
			}
		}
	}
	Vector4f retrievePixel(int lineoffset, int x, int xOffset)
	{
		if (xOffset == 3)
		{
			return Vector4f(texture[lineoffset + x] / 255.0f, texture[lineoffset + x + 1] / 255.0f, texture[lineoffset + x + 2] / 255.0f, 1);
		}
		else
		{
			return Vector4f(texture[lineoffset + x] / 255.0f, texture[lineoffset + x + 1] / 255.0f, texture[lineoffset + x + 2] / 255.0f, texture[lineoffset + x + 3] / 255.0f);
		}
	}
};

struct CubeMap
{
	TexturePtr leftTexturePtr;
	TexturePtr rightTexturePtr;
	TexturePtr topTexturePtr;
	TexturePtr bottomTexturePtr;
	TexturePtr frontTexturePtr;
	TexturePtr backTexturePtr;

	int xOffset; //Bytecount per pixel
	int tWidth;
	int tHeight;
	int pitch;
	CubeMap(TexturePtr leftTexturePtr, TexturePtr rightTexturePtr, TexturePtr topTexturePtr,
	TexturePtr bottomTexturePtr, TexturePtr frontTexturePtr, TexturePtr backTexturePtr)
	{
		this->leftTexturePtr = leftTexturePtr;
		this->rightTexturePtr = rightTexturePtr;
		this->topTexturePtr = topTexturePtr;
		this->bottomTexturePtr = bottomTexturePtr;
		this->frontTexturePtr = frontTexturePtr;
		this->backTexturePtr = backTexturePtr;

		//Assuming all the textures are identical (otherwise this won't work very well)
		tWidth = leftTexturePtr->clip_rect.w;
		tHeight = leftTexturePtr->clip_rect.h;
		pitch = leftTexturePtr->pitch;
		xOffset = pitch / tWidth;

	}
};

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

	TextureWrapper ambientTexture;
	TextureWrapper diffuseTexture;
	TextureWrapper specularTexture;
	Material() {};
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

	bool isClosed = true;

	//Texture and light info from mtl file
	Material material;

	void normalize();
	void updateTranslationMatrix();
	Object();
	~Object();
};

