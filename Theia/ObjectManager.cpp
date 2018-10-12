#include "stdafx.h"
#include "ObjectManager.h"
#pragma once
#include "Primitive.h"
#include <filesystem>
#include <iostream>
#include <experimental\filesystem>
#include "TextureManager.h"

namespace fs = std::experimental::filesystem;

using namespace std;


//Get all files in a directory, might be useful
//string dir = FileHandler::getRunningDir()+"/Resources/Meshes";
//for (auto & p : fs::directory_iterator(dir))
//graphicsObjects[p.path().string().substr(dir.size()+1)] = FileHandler::loadObjectFromFile(p.path().string());


Object ObjectManager::loadObjectFromFile(std::string filename)
{
	ifstream file(dir + "/Resources/Objects/" + filename + "/" + filename + ".obj");
	Object object = Object();
	object.vertices = Vertices(100);

	string line;
	vector<string> splitLine;
	vector<string> vertexAInfoSplit;
	vector<string> vertexBInfoSplit;
	vector<string> vertexCInfoSplit;
	vector<string> vertexDInfoSplit;

	int vertexCount = 0;
	int UVCount = 0;
	int normalCount = 0;
	int colorCount = 0;

	while (file.good())
	{
		getline(file, line);
		if (line.size() > 3 && line[0] != '#')
		{
			splitLine = FileHandler::split(line, ' ');
			if (splitLine[0] == "v") //Vertex data
			{
				object.vertices.points(0, vertexCount) = strtof((splitLine[1]).c_str(), 0);
				object.vertices.points(1, vertexCount) = strtof((splitLine[2]).c_str(), 0);
				object.vertices.points(2, vertexCount) = strtof((splitLine[3]).c_str(), 0);
				object.vertices.points(3, vertexCount) = 1;
				vertexCount++;
			}
			else if (splitLine[0] == "vt")
			{
				object.vertices.UVCoords(0, UVCount) = strtof((splitLine[1]).c_str(), 0);
				object.vertices.UVCoords(1, UVCount) = strtof((splitLine[2]).c_str(), 0);
				UVCount++;

			}
			else if (splitLine[0] == "vn")
			{
				object.vertices.normals(0, normalCount) = strtof((splitLine[1]).c_str(), 0);
				object.vertices.normals(1, normalCount) = strtof((splitLine[2]).c_str(), 0);
				object.vertices.normals(2, normalCount) = strtof((splitLine[3]).c_str(), 0);
				object.vertices.normals(3, normalCount) = 1;
				normalCount++;
			}
			else if (splitLine[0] == "f") //Face data
			{
				if (splitLine.size() == 4) //Triangles
				{
					vertexAInfoSplit = FileHandler::split(splitLine[1], '/');
					vertexBInfoSplit = FileHandler::split(splitLine[2], '/');
					vertexCInfoSplit = FileHandler::split(splitLine[3], '/');
					if (vertexAInfoSplit.size() == 1)
					{
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1));
					}
					else
					{
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexBInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexBInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1));
					}
				}
				else //Quads
				{
					//Add two faces
					vertexAInfoSplit = FileHandler::split(splitLine[1], '/');
					vertexBInfoSplit = FileHandler::split(splitLine[2], '/');
					vertexCInfoSplit = FileHandler::split(splitLine[3], '/');
					vertexDInfoSplit = FileHandler::split(splitLine[4], '/');
					if (vertexAInfoSplit.size() == 1)
					{
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1));
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexDInfoSplit[0]) - 1));
					} //No texture or normal
					else
					{
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexBInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexBInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1));
						object.primitives.push_back(
							Primitive(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexDInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexDInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1,
								stoi(vertexDInfoSplit[2]) - 1));
					} //No texture or normal
				}
			}
			else if (splitLine[0] == "mtllib") //Load material file for object
			{
				object.material = loadMaterialFromFile(filename, splitLine[1]);
				object.material.isTextured = true;
			}
			int matrixSize = object.vertices.getVertexCount();
			if (vertexCount >= matrixSize || normalCount >= matrixSize || UVCount >= matrixSize || colorCount >= matrixSize)
				object.vertices.resize(object.vertices.getVertexCount() * 2);
		}
	}
	file.close();
	std::cout << "Loaded object: " << filename << std::endl;
	return object;
}

Material ObjectManager::loadMaterialFromFile(std::string objectname, ::string filename)
{
	ifstream file(dir + "/Resources/Objects/" + objectname + "/" + filename);
	Material material;
	string line;
	vector<string> splitLine;
	while (file.good())
	{
		getline(file, line);
		if (line.size() > 1 && line[0] != '#')
		{
			splitLine = FileHandler::split(line, ' ');
			if (splitLine[0] == "newmtl")
			{
				material.name = splitLine[1];
			}
			else if (splitLine[0] == "Ns")
			{
				material.specularExp = strtof((splitLine[1]).c_str(), 0);
			}
			else if (splitLine[0] == "d")
			{
				material.transparency = strtof((splitLine[1]).c_str(), 0);
			}
			else if (splitLine[0] == "Tf")
			{
				material.tFilter = Vector3f(strtof((splitLine[1]).c_str(), 0), strtof((splitLine[2]).c_str(), 0), strtof((splitLine[3]).c_str(), 0));
			}
			else if (splitLine[0] == "illum")
			{
				material.illumType = stoi(splitLine[1]);
			}
			else if (splitLine[0] == "Ka")
			{
				material.ambient = Vector3f(strtof((splitLine[1]).c_str(), 0), strtof((splitLine[2]).c_str(), 0), strtof((splitLine[3]).c_str(), 0));
			}
			else if (splitLine[0] == "Kd")
			{
				material.diffuse = Vector3f(strtof((splitLine[1]).c_str(), 0), strtof((splitLine[2]).c_str(), 0), strtof((splitLine[3]).c_str(), 0));
			}
			else if (splitLine[0] == "Ks")
			{
				material.specular = Vector3f(strtof((splitLine[1]).c_str(), 0), strtof((splitLine[2]).c_str(), 0), strtof((splitLine[3]).c_str(), 0));
			}
			else if (splitLine[0] == "Ke")
			{
				material.emissive = Vector3f(strtof((splitLine[1]).c_str(), 0), strtof((splitLine[2]).c_str(), 0), strtof((splitLine[3]).c_str(), 0));
			}
			else if (splitLine[0] == "map_Ka")
			{
				material.ambientTexture = txMng->load(objectname, splitLine[1]);
			}
			else if (splitLine[0] == "map_Kd")
			{
				material.diffuseTexture = txMng->load(objectname, splitLine[1]);
			}
			else if (splitLine[0] == "map_Ks")
			{
				material.specularTexture = txMng->load(objectname, splitLine[1]);
			}
		}
	}
	cout << "Loaded material: " << material.name << endl;
	return material;
}

ObjectPtr ObjectManager::load(const std::string & filename, bool normalizeUponLoad)
{
	// First lookup if the texture with that filename
	// is already loaded, if yes, return it instead
	// so that it can be shared.
	auto objIterator = loadedObjects.find(filename);
	if (objIterator != loadedObjects.end())
		return objIterator->second;

	// Texture with that filename is not loaded yet.
	// Load it and save it into the map cache.
	Object newObject = loadObjectFromFile(filename);
	if (normalizeUponLoad) newObject.normalize();
	ObjectPtr newObjectPtr = std::make_shared<Object>(newObject);

	loadedObjects.insert(std::make_pair(filename, newObjectPtr));

	return newObjectPtr;
}

ObjectManager::ObjectManager()
{
}
