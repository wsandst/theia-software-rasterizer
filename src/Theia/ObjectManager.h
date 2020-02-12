#pragma once
#include <map>
#include "Object.h"
#include "FileHandler.h"
#include <string>

using namespace std;

typedef std::shared_ptr<Object> ObjectPtr;

class ObjectManager
{
public:
	std::string dir;
	TextureManager *txMng;
	ObjectManager(ObjectManager const&) = delete;
	ObjectManager& operator=(ObjectManager const&) = delete;

	Object loadObjectFromFile(std::string filename);
	Material loadMaterialFromFile(std::string objectname, ::string filename);
	ObjectPtr load(const std::string & filename, bool normalizeUponLoad = false);

	ObjectManager();

private:

	std::map<std::string, ObjectPtr> loadedObjects;
};

