#include "Hop32.h"
#include "HopImpl.h"

float Hop::s_size = 1.0f;
float Hop::s_x = 0.0f;
float Hop::s_y = 0.0f;
float Hop::s_z = 0.0f;
bool Hop::s_isInvert(false);

float Hop::getSize()
{
	return s_size;
}

float Hop::getX()
{
	return s_x;
}

float Hop::getY()
{
	return s_y;
}

float Hop::getZ()
{
	return s_z;
}
bool Hop::isInvert()
{
	return s_isInvert;
}

void Hop::setSize(float size)
{
	s_size = size;
}

void Hop::setPosition(float x, float y, float z)
{
	s_x = x;
	s_y = y;
	s_z = z;
}

void Hop::setFactor(float x, float y, float z, float size)
{
	setPosition(x, y, z);
	setSize(size);
}

void Hop::setInvert(bool isInvert)
{
	s_isInvert = isInvert;
}

void Hop::setBeforeDisplayFunc(void(*beforeCallBack)())
{
	HopImpl::s_before = beforeCallBack;
}

void Hop::setAfterDisplayFunc(void(*afterCallBack)())
{
	HopImpl::s_after = afterCallBack;
}

void Hop::setDisplayFunc(void(*displayCallBack)(HopSide))
{
	HopImpl::s_display = displayCallBack;
	HopImpl::registerCallBack();
}

void Hop::setReshapeFunc(void(*reshapeCallBack)(int, int))
{
	HopImpl::s_reshape = reshapeCallBack;
}
