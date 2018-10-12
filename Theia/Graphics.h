#pragma once
#include <vector>
#include "Framebuffer.h"
#include "Camera.h"

class Graphics
{
private:
	Framebuffer screen;
	Camera view;
public:
	void setScreenSize(int width, int height);
	Graphics();
	~Graphics();
};

