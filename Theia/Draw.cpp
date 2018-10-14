#include "stdafx.h"
#include "Draw.h"


Framebuffer Draw::drawFragments(Framebuffer framebuffer, std::vector<Fragment> fragments)
{
	for (size_t i = 0; i < fragments.size(); i++)
	{
		Fragment frag = fragments[i];
		//Z-check
		//cout << to_string(framebuffer.getDepthValue((int)frag.point[0], (int)frag.point[1])) << " < " << to_string(frag.point[2]) << endl;
		if (framebuffer.getDepthValue((int)frag.point[0], (int)frag.point[1]) < frag.point[2]) continue;
		framebuffer.setDepthValue((int)frag.point[0], (int)frag.point[1], frag.point[2]);
		framebuffer.setColorValue((int)frag.point[0], (int)frag.point[1], frag.outValue);
		//Stencil here once I start doing that
	}
	return framebuffer;
}

//Do not modify, static class
Draw::Draw()
{
}


Draw::~Draw()
{
}
