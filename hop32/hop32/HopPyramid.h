#ifndef _HOP_PYRAMID_H
#define _HOP_PYRAMID_H

class HopRenderTarget;

class HopPyramid
{

private:

	unsigned int m_vao;

	int m_attrPosition;
	int m_attrTexCoord;
	int m_uniformTexture;

public:

	HopPyramid();
	~HopPyramid();

	void init();
	void draw(HopRenderTarget renderTarget[]);
};

#endif