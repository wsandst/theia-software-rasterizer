#pragma once
class Draw
{
	void drawFragments(unsigned char *frameBuffer); //Does blending and depth testing
	void drawWireframe(unsigned char *frameBuffer); //Draws a wireframe from a Vertices object
	void drawLine(unsigned char *frameBuffer);
public:
	Draw();
	~Draw();
};

