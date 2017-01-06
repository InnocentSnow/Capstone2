#include <Windows.h>
#include "keyboard.h"

float Keyboard::getX()
{
	if (GetKeyState('Z') & 0x8000)
	{
		return 1;
	}

	return 0;
}

float Keyboard::getY()
{
	if (GetKeyState('X') & 0x8000)
	{
		return 1;
	}

	return 0;
}

float Keyboard::getZ()
{
	if (GetKeyState('C') & 0x8000)
	{
		return 1;
	}

	return 0;
}
