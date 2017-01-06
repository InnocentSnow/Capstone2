#ifndef _HOP32_H
#define _HOP32_H

#ifdef HOP32_EXPORTS
#define HOP32_API __declspec(dllexport)
#else
#define HOP32_API __declspec(dllimport)
#endif

namespace hop
{
	void hopDisplay();
};

class HOP32_API HopSide
{

private:

	float m_eye[3];
	float m_at[3];

	HopSide(float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ);

	friend void hop::hopDisplay();

public:

	void getViewMatrix(float out[]);
	void getProjectionMatrix(float out[]);
	void getViewProjectionMatrix(float out[]);

	void gluPerspective();
	void gluLookAt();
};

class Hop
{

public:

	HOP32_API static void setBeforeDisplayFunc(void(*beforeCallBack)());
	HOP32_API static void setAfterDisplayFunc(void(*afterCallBack)());

	HOP32_API static void setDisplayFunc(void(*displayCallBack)(HopSide));
	HOP32_API static void setReshapeFunc(void(*reshapeCallBack)(int, int));

	HOP32_API static float getSize();
	HOP32_API static float getX();
	HOP32_API static float getY();
	HOP32_API static float getZ();
	HOP32_API static bool isInvert();

	HOP32_API static void setSize(float size);
	HOP32_API static void setPosition(float x, float y, float z);
	HOP32_API static void setFactor(float x, float y, float z, float size);
	HOP32_API static void setInvert(bool isInvert);

private:

	static float s_size;
	static float s_x;
	static float s_y;
	static float s_z;
	static bool s_isInvert;

	Hop();
};

#endif