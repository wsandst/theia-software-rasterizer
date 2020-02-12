#pragma once
#include <memory>
#include <iostream>
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_image.h"

// Our texture "handle"
typedef std::shared_ptr<SDL_Surface> TexturePtr;

class TextureManager
{
public:
	std::string dir;
	TextureManager(TextureManager const&) = delete;
	TextureManager& operator=(TextureManager const&) = delete;

	SDL_Surface loadFromFile(std::string objectname, std::string filename);
	TexturePtr load(const std::string objectname, const std::string & filename);

	TextureManager();

private:

	std::map<std::string, TexturePtr> loadedTextures;
};

