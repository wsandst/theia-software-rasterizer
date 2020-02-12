#include "Draw.h"


void Draw::drawFragments(Framebuffer& framebuffer, std::vector<Fragment>& fragments)
{
	//Blending
	for (size_t i = 0; i < fragments.size(); i++)
	{
		Fragment frag = fragments[i];
		//Z-check
		//cout << to_string(framebuffer.getDepthValue((int)frag.point[0], (int)frag.point[1])) << " < " << to_string(frag.point[2]) << endl;
		if (framebuffer.getDepthValue(frag.point[0], frag.point[1]) < frag.point[2]) continue;
		if (frag.outValue[3] > 0.99) //If not transparent, no need to blend
		{

			framebuffer.setDepthValue(frag.point[0], frag.point[1], frag.point[2]);
			framebuffer.setColorValue(frag.point[0], frag.point[1], frag.outValue);
		}
		else
		{
			framebuffer.setDepthValue(frag.point[0], frag.point[1], frag.point[2]);
			Vector4f color = blend(frag.outValue, framebuffer.getColorValue(frag.point[0], frag.point[1]));
			framebuffer.setColorValue(frag.point[0], frag.point[1], color);
		}
		//Stencil here once I start doing that
	}
}
Vector4f Draw::blend(Vector4f sourceColor, Vector4f destColor)
{
	//Blending. You have to manually change the blending function for now if you want to do that
	//Cf = (Cs * S) + (Cd * D)
	Vector4f S = Vector4f(sourceColor[3], sourceColor[3], sourceColor[3], sourceColor[3]);
	Vector4f D = Vector4f(1, 1, 1, 1) - S;
	return (sourceColor.cwiseProduct(S)) + (destColor.cwiseProduct(D));
}
//Do not modify, static class
Draw::Draw()
{
}


Draw::~Draw()
{
}
