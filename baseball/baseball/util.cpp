#include <Windows.h>
#include <math.h>
#include "data.h"

float calcDist(const Position &p1, const Position &p2)
{
	return (float)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}

float calcHorizontalDist(const Position &p1, const Position &p2)
{
	return (float)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.z - p2.z) * (p1.z - p2.z));
}

float convertKMPHToMPMS(float kmps)
{
	return kmps / 3600;
}

DWORD getTime()
{
	return GetTickCount();
}