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

typedef void(*functionPtr)(int);

class Graphics
{
public:
	Framebuffer screen;
	Camera view;
	vector<ObjectPtr> worldObjects;
		 
	void setScreenSize(int width, int height);
	void addToObjectToPipeline(ObjectPtr object);
	Vertices createWorldVertices();
	Vertices applyVertexShading(Vertices vertices, functionPtr shader);
	vector<Fragment> generateFragments(Vertices vertices, ObjectPtr object);

	void renderMainView();
	void setup();
	unsigned char* getDisplaybufferPtr();
	void clearDisplaybuffer();

	Graphics();
	~Graphics();
};

