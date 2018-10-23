#include "stdafx.h"
#include "Graphics.h"


void Graphics::setScreenSize(int width, int height)
{
	screen = Framebuffer(width, height);
}

void Graphics::addToObjectToPipeline(ObjectPtr object)
{
	worldObjects.push_back(object);
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

void Graphics::generateFragments(vector<Fragment> &fragments, Vertices vertices, ObjectPtr object)
{
	Primitive prim;
	fragments.reserve((int)(fragmentCountLastFrame * 1.2));
	//Predict size somehow? Hmm. From last iteration? Genius
	for (size_t i = 0; i < object->primitives.size(); i++)
	{
		prim = object->primitives[i];
		Vertex vertex1 = vertices.getVertex(prim.points[0], prim.normals[0], prim.UVcoords[0], prim.colors[0]);
		Vertex vertex2 = vertices.getVertex(prim.points[1], prim.normals[1], prim.UVcoords[1], prim.colors[1]);
		Vertex vertex3 = vertices.getVertex(prim.points[2], prim.normals[2], prim.UVcoords[2], prim.colors[2]);
		if (drawMode == WIREFRAME)
			Rasterizer::createLineFragments(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		else if (drawMode == POLY)
			Rasterizer::createPolyFragments(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		else if (drawMode == DEBUG)
		{
			Rasterizer::createDebugFragments(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height);
		}
	}
	fragmentCountLastFrame = fragments.size();
}

void Graphics::renderMainView()
{
	view.calculateCameraViewMatrix();
	//Render every object
	for (size_t i = 0; i < worldObjects.size(); i++)
	{
		Vertices cameraVertices = VertexShaders::perspectiveShader(worldObjects[i]->vertices, view.cameraViewMatrix);
		vector<Fragment> fragments;
		generateFragments(fragments, cameraVertices, worldObjects[i]);

		//FragmentShaders::colorAndNormalShader(fragments, Vector3f(1, 0, -1), Vector4f(1, 0.7, 0.35, 1));
		FragmentShaders::textureShader(fragments, worldObjects[i]->material.ambientTexture);
		//FragmentShaders::textureAndColor(fragments, worldObjects[i]->material.ambientTexture);
		//FragmentShaders::simpleColorShader(fragments);
		//FragmentShaders::textureLightShader(fragments, worldObjects[i]->material.ambientTexture, Vector3f(1, 0, -1), Vector4f(1, 1, 1, 1));

		//Now to draw the fragments to the framebuffer.
		screen = Draw::drawFragments(screen, fragments);
	}
	screen.generateOutputBuffer();

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
