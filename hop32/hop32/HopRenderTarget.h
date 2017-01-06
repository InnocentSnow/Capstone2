#ifndef _HOP_RENDER_TARGET_H
#define _HOP_RENDER_TARGET_H

class HopRenderTarget
{

private:

	enum
	{
		RENDER_TARGET_WIDTH = 1024,
		RENDER_TARGET_HEIGHT = 1024
	};

	unsigned int m_frameBuffer;
	unsigned int m_depthBuffer;
	unsigned int m_texture;

public:

	HopRenderTarget();
	~HopRenderTarget();

	void init();

	void applyTexture();
	void applyRenderTarget();

	static void cancelRenderTarget(int width, int height);
};

#endif