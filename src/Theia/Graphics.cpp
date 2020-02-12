#include "Graphics.h"


void Graphics::setScreenSize(int width, int height)
{
	screen = Framebuffer(width, height);
	earlyZBuffer = Framebuffer(width, height);
}

void Graphics::addToObjectToPipeline(ObjectPtr object)
{
	worldObjects.push_back(object);
	objectFragments.push_back(vector<Fragment>());
}

void Graphics::generateFragments(vector<Fragment>& fragments, Vertices vertices, ObjectPtr object)
{
	Primitive prim;
	Vector4f surfaceNormal;
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
			if (backfaceCulling && object->isClosed) //Backface culling
			{
				surfaceNormal = vertices.surfaceNormals.col(object->primitives[i].surfaceNormal);
				if (vertex1.point.dot(surfaceNormal) >= 0) continue;
			}
			Rasterizer::createPolyFragments2(&fragments, vertex1, vertex2, vertex3, screen.width, screen.height, earlyZBuffer, earlyZTest);
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

	view.updateMatrices();
	//Render every object
	Vertices objectVertices;
	for (size_t i = 0; i < worldObjects.size(); i++)
	{
		objectVertices = VertexShaders::projectionShader(worldObjects[i]->vertices, view, worldObjects[i]->modelMatrix);
		generateFragments(objectFragments[i], objectVertices, worldObjects[i]);

		FragmentShaders::textureShader(objectFragments[i], worldObjects[i]->material.ambientTexture);
		//FragmentShaders::simpleColorShader(objectFragments[i]);
	}
	for (size_t i = 0; i < objectFragments.size(); i++) //Draw all fragments
	{
		//std::sort(objectFragments[i].begin(), objectFragments[i].end());
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
	//Optimizations
	backfaceCulling = false;
	earlyZTest = false;
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
	earlyZBuffer.clearDepthBuffer();
}

Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}
