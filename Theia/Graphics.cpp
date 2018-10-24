#include "stdafx.h"
#include "Graphics.h"


void Graphics::setScreenSize(int width, int height)
{
	screen = Framebuffer(width, height);
}

void Graphics::addToObjectToPipeline(ObjectPtr object)
{
	worldObjects.push_back(object);
	objectFragments.push_back(vector<Fragment>());
}

Vertices Graphics::createWorldVertices()
{
	//Bottleneck probably. I could do this with block operations instead maybe, to decrease the amount of new objects needed
	Vertices worldVertices = VertexShaders::transformShader(worldObjects[0]->vertices, worldObjects[0]->localTranslationMatrix);
	for (size_t i = 1; i < worldObjects.size(); i++)
	{
		Vertices objectVertices = VertexShaders::transformShader(worldObjects[i]->vertices, worldObjects[i]->localTranslationMatrix);
		worldVertices.combine(objectVertices);
	}
	return worldVertices;
}

void Graphics::generateFragments(vector<Fragment>& fragments, Vertices vertices, ObjectPtr object)
{
	Primitive prim;
	//Predict size somehow? Hmm. From last iteration? Genius
	for (size_t i = 0; i < object->primitives.size(); i++)
	{
		prim = object->primitives[i];
		Vertex vertex1 = vertices.getVertex(prim.points[0], prim.normals[0], prim.UVcoords[0], prim.colors[0]);
		Vertex vertex2 = vertices.getVertex(prim.points[1], prim.normals[1], prim.UVcoords[1], prim.colors[1]);
		Vertex vertex3 = vertices.getVertex(prim.points[2], prim.normals[2], prim.UVcoords[2], prim.colors[2]);
		if (drawMode == WIREFRAME)
		{
			Rasterizer::createLineFragments(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		}
		else if (drawMode == POLY)
		{
			Rasterizer::createPolyFragments2(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		}
		else if (drawMode == DEBUG)
		{
			Rasterizer::createDebugFragments(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		}
	}
}

void Graphics::renderMainView()
{
	//Performance stuff
	memoryManagement();

	view.calculateCameraViewMatrix();
	//Render every object
	Vertices objectVertices, cameraVertices;
	for (size_t i = 0; i < worldObjects.size(); i++)
	{
		objectVertices = VertexShaders::transformShader(worldObjects[i]->vertices, worldObjects[i]->localTranslationMatrix);
		cameraVertices = VertexShaders::perspectiveShader(objectVertices, view.cameraViewMatrix);
		generateFragments(objectFragments[i], cameraVertices, worldObjects[i]);

		FragmentShaders::textureShader(objectFragments[i], worldObjects[i]->material.ambientTexture);
		//FragmentShaders::simpleColorShader(fragments);
	}
	for (size_t i = 0; i < objectFragments.size(); i++) //Draw all fragments
	{
		std::sort(objectFragments[i].begin(), objectFragments[i].end());
		Draw::drawFragments(screen, objectFragments[i]);
	}

	screen.generateOutputBuffer();
}

void Graphics::memoryManagement()
{
	for (size_t i = 0; i < objectFragments.size(); i++)
	{
		int fragCount = objectFragments[i].size();
		objectFragments[i].clear();
		objectFragments[i].reserve((int)(fragCount * 1.2));
	}
}

void Graphics::setup()
{
	//Setup stuff
	drawMode = POLY;
	view.setViewport(screen.width, screen.height);
	view.setTranslation(Vector3f(0, 0, -3));
	view.setRotation(Vector3f(3.14, 3.14, 0));
	view.setScale(1);
}

float* Graphics::getDisplaybufferPtr()
{
	return &screen.colorBuffer[0];
}

void Graphics::clearDisplaybuffer()
{
	screen.clear();
}

Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}
