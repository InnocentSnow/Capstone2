#include <Windows.h>
#include <gl/glut.h>
#include <assert.h>
#include "Hop32.h"
#include "HopImpl.h"
#include "HopRenderTarget.h"
#include "HopPyramid.h"
#include "HopShaderMgr.h"
#include "HopError.h"

bool HopImpl::s_isRegistered = false;
int HopImpl::s_width = 128;
int HopImpl::s_height = 128;

HopImpl::BeforeCallBack *HopImpl::s_before = 0;
HopImpl::AfterCallBack *HopImpl::s_after = 0;
HopImpl::DisplayCallBack *HopImpl::s_display = 0;
HopImpl::ReshapeCallBack *HopImpl::s_reshape = 0;
HopRenderTarget HopImpl::s_renderTarget[HopImpl::NUM_RENDER_TARGET];
HopPyramid HopImpl::s_pyramid;

static HWND getHwnd()
{
	HWND hwnd = GetActiveWindow();
	HWND parent;

	while (NULL != (parent = GetParent(hwnd)))
	{
		hwnd = parent;
	}

	return hwnd;
}

static void changeToFullscreen()
{
	HWND hwnd = getHwnd();
	if (hwnd == NULL)
	{
		hopError("Failed to get HWND\n");
		abort();
	}

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

	ShowWindow(hwnd, SW_MAXIMIZE);
}

namespace hop
{
	void hopDisplay()
	{
		static bool firsttime = true;
		if (firsttime)
		{
			firsttime = false;
			changeToFullscreen();
		}

		//Left Right Front Back
		static float dX[] = { 1, -1, 0, 0 }; //mirrored
		static float dZ[] = { 0, 0, 1, -1 };
		static float Y_RATE = 0.4f;

		float x = Hop::getX();
		float y = Hop::getY();
		float z = Hop::getZ();
		float size = Hop::getSize() / 2;
		float cy = y + size * Y_RATE;

		assert(HopImpl::s_display != 0);
		if (HopImpl::s_before) HopImpl::s_before();

		for (int i = 0; i < HopImpl::NUM_RENDER_TARGET; ++i)
		{
			if(Hop::isInvert()) HopImpl::s_renderTarget[i % 2 ? i - 1 : i + 1].applyRenderTarget();
			else HopImpl::s_renderTarget[i].applyRenderTarget();
			HopImpl::s_display(HopSide(x + dX[i] * size, cy, z + dZ[i] * size, x, cy, z));
		}

		HopRenderTarget::cancelRenderTarget(HopImpl::s_width, HopImpl::s_height);
		
		GLfloat savedClearColor[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, savedClearColor);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		HopImpl::s_pyramid.draw(HopImpl::s_renderTarget);
		glutSwapBuffers();
		
		glClearColor(savedClearColor[0], savedClearColor[1], savedClearColor[2], savedClearColor[3]);
		if (HopImpl::s_after) HopImpl::s_after;
	}

	void hopReshape(int width, int height)
	{
		HopImpl::s_width = width;
		HopImpl::s_height = height;

		int size = width < height ? width : height;
		if (HopImpl::s_reshape) HopImpl::s_reshape(size, size);
	}
};

void HopImpl::initialize()
{
	HopShaderMgr::getInstance()->load();
	s_pyramid.init();
	
	for (int i = 0; i < NUM_RENDER_TARGET; ++i)
	{
		s_renderTarget[i].init();
	}
}

void HopImpl::registerCallBack()
{
	if (!s_isRegistered) initialize();
	s_isRegistered = true;

	glutReshapeFunc(hop::hopReshape);
	glutDisplayFunc(hop::hopDisplay);
}
