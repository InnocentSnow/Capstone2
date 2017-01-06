#ifndef _HOP_H
#define _HOP_H

class HopSide
{

public:

	void getViewMatrix(float out[]);
	void getProjectionMatrix(float out[]);
	void getViewProjectionMatrix(float out[]);

	void gluPerspective();
	void gluLookAt();

private:

	HopSide();

};

class Hop
{

public:

	static void setBeforeDisplayFunc(void(*beforeCallBack)());
	static void setAfterDisplayFunc(void(*afterCallBack)());

	static void setDisplayFunc(void(*displayCallBack)(HopSide));
	static void setReshapeFunc(void(*reshapeCallBack)(int, int));

	static float getSize();
	static float getX();
	static float getY();
	static float getZ();
	static bool isInvert();

	static void setSize(float size);
	static void setPosition(float x, float y, float z);
	static void setFactor(float x, float y, float z, float size);
	static void setInvert(bool isInvert);

private:

	Hop();
};

#endif
