#include "stdafx.h"
#include "TextureManager.h"

TexturePtr TextureManager::load(const std::string objectname, const std::string & filename)
{
	// First lookup if the texture with that filename
	// is already loaded, if yes, return it instead
	// so that it can be shared.
	auto texIterator = loadedTextures.find(filename);
	if (texIterator != loadedTextures.end())
		return texIterator->second;

	// Texture with that filename is not loaded yet.
	// Load it and save it into the map cache.
	SDL_Surface newTexture = loadFromFile(objectname, filename);
	TexturePtr newTexturePtr = std::make_shared<SDL_Surface>(newTexture);

	loadedTextures.insert(std::make_pair(filename, newTexturePtr));

	return newTexturePtr;
}

TextureManager::TextureManager()
{
}

SDL_Surface TextureManager::loadFromFile(std::string objectname, std::string filename)
{
	std::string newFileName = dir + "\\Resources\\Objects\\" + objectname + "\\" + filename;
	const char* file = newFileName.c_str();
	SDL_Surface *surface = IMG_Load(file);
	std::cout << "Loaded texture: " << filename << std::endl;
	return *surface;
}
