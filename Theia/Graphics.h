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

	bool backfaceCulling = true;
	bool earlyZTest = true;

	DrawMode drawMode;

	Framebuffer screen;
	Camera view;
	vector<ObjectPtr> worldObjects;
		 
	vector<vector<Fragment>> objectFragments;
	Framebuffer earlyZBuffer;

	void setScreenSize(int width, int height);
	void addToObjectToPipeline(ObjectPtr object);
	Vertices createWorldVertices();
	void generateFragments(vector<Fragment>& fragments, Vertices vertices, ObjectPtr object);

	void renderMainView();
	void memoryManagement();
	void setup();
	float* getDisplaybufferPtr();
	void clearDisplaybuffer();

	Graphics();
	~Graphics();
private:
	int fragmentCountLastFrame = 0;
};

