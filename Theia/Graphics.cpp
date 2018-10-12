#include "stdafx.h"
#include "Graphics.h"


void Graphics::setScreenSize(int width, int height)
{
	screen = Framebuffer(width, height);
}

Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}
