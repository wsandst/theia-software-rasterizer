#pragma once
#include <vector>
#include <iostream>

#include "Framebuffer.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Fragments.h"

#include "VertexShaders.h"
#include "FragmentShaders.h"
#include "Rasterizer.h"
#include "Draw.h"

enum DrawMode
{
	POLY, WIREFRAME, DEBUG
};
class Graphics
{
public:

	DrawMode drawMode;

	Framebuffer screen;
	Camera view;
	vector<ObjectPtr> worldObjects;
		 
	void setScreenSize(int width, int height);
	void addToObjectToPipeline(ObjectPtr object);
	Vertices createWorldVertices();
	void generateFragments(vector<Fragment> &fragments, Vertices vertices, ObjectPtr object);

	void renderMainView();
	void setup();
	float* getDisplaybufferPtr();
	void clearDisplaybuffer();

	Graphics();
	~Graphics();
private:
	int fragmentCountLastFrame = 0;
};

