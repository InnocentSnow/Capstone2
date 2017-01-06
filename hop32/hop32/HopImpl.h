#ifndef _HOP_IMPL_H
#define _HOP_IMPL_H

namespace hop
{
	void hopDisplay();
	void hopReshape(int width, int height);
};

class Hop;
class HopSide;
class HopPyramid;
class HopRenderTarget;

class HopImpl
{

private:

	enum
	{
		NUM_RENDER_TARGET = 4
	};

	HopImpl();

	static void initialize();
	static void registerCallBack();

	typedef void(BeforeCallBack)();
	typedef void(AfterCallBack)();
	typedef void(DisplayCallBack)(HopSide);
	typedef void(ReshapeCallBack)(int, int);

	static HopPyramid s_pyramid;
	static HopRenderTarget s_renderTarget[];
	static bool s_isRegistered;
	static int s_width;
	static int s_height;

	static BeforeCallBack *s_before;
	static AfterCallBack *s_after;
	static DisplayCallBack *s_display;
	static ReshapeCallBack *s_reshape;

	friend class Hop;
	friend void hop::hopDisplay();
	friend void hop::hopReshape(int width, int height);
};

#endif