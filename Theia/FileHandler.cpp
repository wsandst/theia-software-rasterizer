#include "FileHandler.h"



string FileHandler::getRunningDir()
{
	char buf[256];
	windows::GetCurrentDirectoryA(256, buf);
	return string(buf);
}

Object FileHandler::loadObjectFromFile(string dir)
{
	ifstream file(dir);
	Object object = Object();
	object.faces = vector <Face>();
	int matrixSize = 100;
	object.vertices = Matrix4Xf();
	object.normals = Matrix4Xf();
	object.UVCoords = Matrix2Xf();
	object.vertices.resize(Eigen::NoChange, 100);
	object.normals.resize(Eigen::NoChange, 100);
	object.UVCoords.resize(Eigen::NoChange, 100);
	string line;
	vector<string> splitLine;
	vector<string> vertexAInfoSplit;
	vector<string> vertexBInfoSplit;
	vector<string> vertexCInfoSplit;
	vector<string> vertexDInfoSplit;

	while (file.good())
	{
		getline(file, line);
		if (line.size() > 3 && line[0] != '#')
		{
			splitLine = split(line, ' ');
			if (splitLine[0] == "v") //Vertex data
			{
				object.vertices(0, object.vertexCount) = strtof((splitLine[1]).c_str(), 0);
				object.vertices(1, object.vertexCount) = strtof((splitLine[2]).c_str(), 0);
				object.vertices(2, object.vertexCount) = strtof((splitLine[3]).c_str(), 0);
				object.vertices(3, object.vertexCount) = 1;
				object.vertexCount++;
				if (object.vertexCount >= object.vertices.cols())
					object.vertices.conservativeResize(Eigen::NoChange, object.vertices.cols() * 2);
			}
			else if (splitLine[0] == "vt")
			{
				object.UVCoords(0, object.UVCount) = strtof((splitLine[1]).c_str(), 0);
				object.UVCoords(1, object.UVCount) = strtof((splitLine[2]).c_str(), 0);
				object.UVCount++;
				if (object.UVCount >= object.UVCoords.cols())
					object.UVCoords.conservativeResize(Eigen::NoChange, object.UVCoords.cols() * 2);
			}
			else if (splitLine[0] == "vn")
			{
				object.normals(0, object.normalCount) = strtof((splitLine[1]).c_str(), 0);
				object.normals(1, object.normalCount) = strtof((splitLine[2]).c_str(), 0);
				object.normals(2, object.normalCount) = strtof((splitLine[3]).c_str(), 0);
				object.normalCount++;
				if (object.normalCount >= object.normals.cols())
					object.normals.conservativeResize(Eigen::NoChange, object.normals.cols() * 2);
			}
			else if (splitLine[0] == "f") //Face data
			{
				if (splitLine.size() == 4) //Triangles
				{
					vertexAInfoSplit = split(splitLine[1], '/');
					vertexBInfoSplit = split(splitLine[2], '/');
					vertexCInfoSplit = split(splitLine[3], '/');
					if (vertexAInfoSplit.size() == 1)
					{
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								Vector3f(1, 1, 1)));
					}
					else
					{
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexBInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexBInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1,
								Vector3f(1, 1, 1)));
						object.faceCount++;
					}
				}
				else //Quads
				{
					//Add two faces
					vertexAInfoSplit = split(splitLine[1], '/');
					vertexBInfoSplit = split(splitLine[2], '/');
					vertexCInfoSplit = split(splitLine[3], '/');
					vertexDInfoSplit = split(splitLine[4], '/');
					if (vertexAInfoSplit.size() == 1)
					{
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								Vector3f(1, 1, 1)));
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexDInfoSplit[0]) - 1,
								Vector3f(1, 1, 1)));
					} //No texture or normal
					else
					{
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexBInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexBInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexBInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1,
								Vector3f(1, 1, 1)));
						object.faces.push_back(
							Face(stoi(vertexAInfoSplit[0]) - 1,
								stoi(vertexCInfoSplit[0]) - 1,
								stoi(vertexDInfoSplit[0]) - 1,
								stoi(vertexAInfoSplit[1]) - 1,
								stoi(vertexCInfoSplit[1]) - 1,
								stoi(vertexDInfoSplit[1]) - 1,
								stoi(vertexAInfoSplit[2]) - 1,
								stoi(vertexCInfoSplit[2]) - 1,
								stoi(vertexDInfoSplit[2]) - 1,
								Vector3f(1, 1, 1)));
					} //No texture or normal
					object.faceCount += 2;
				}
			}
		}

	}
	object.vertices.conservativeResize(4, object.vertexCount);
	file.close();
	return object;
}

vector<string> FileHandler::split(string input, char delimiter)
{
	vector<string> output;
	size_t foundSymbolAt = 0;
	size_t foundSymbolAtOld = -1;
	int i = 0;
	while (foundSymbolAt != -1)
	{
		foundSymbolAt = input.find(delimiter, (int)(foundSymbolAtOld + 1));
		if (input.substr(foundSymbolAtOld + 1, foundSymbolAt - foundSymbolAtOld - 1) != "")
			output.push_back(input.substr(foundSymbolAtOld + 1, foundSymbolAt - foundSymbolAtOld - 1));
		foundSymbolAtOld = foundSymbolAt;
		i++;
	}
	return output;
}

FileHandler::FileHandler()
{
}


FileHandler::~FileHandler()
{
}
