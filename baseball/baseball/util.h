#ifndef _UTIL_H
#define _UTIL_H

#include <Windows.h>

enum
{
	NUM_SLICE = 10,
	NUM_CIRCLE_POINT = 50,
	THROW_TIME = 2000
};

#define STADIUM_RADIUS 100.0f
#define PLAYER_SPEED 0.008f
#define FENCE_HEIGHT 3.5f
#define BALL_SIZE 0.10f
#define FIELD 19.4f
#define PI 3.14159265358979f

struct Position
{
	Position() : Position(0, 0, 0) {}
	Position(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	float x;
	float y;
	float z;
};

template <typename T>
float getPortion(T start, T end, T current)
{
	if (start == end) return 1;
	return (float)(current - start) / (end - start);
}

template <typename T>
T linearInterpolate(T start, T end, float portion)
{
	T diff = end - start;
	return T(start + diff * portion);
}

template <typename T>
T squareInterpolate(T start, T end, float portion)
{
	T diff = end - start;
	return T(start + diff * portion * portion);
}

float calcDist(const Position &p1, const Position &p2);
float convertKMPHToMPMS(float kmps);
float calcHorizontalDist(const Position &p1, const Position &p2);
DWORD getTime();

#endif
