#ifndef _DATA_H
#define _DATA_H

#include <Windows.h>
#include <vector>
#include "util.h"
#include "player.h"

class BallData
{

private:

	Position m_start;
	Position m_end;
	float m_maxY;

	DWORD m_startTime;
	DWORD m_endTime;
	DWORD m_lastTime;

public:
	
	Position getStartPosition() { return m_start; }
	Position getEndPosition() { return m_end; }

	Position getPosition(DWORD currentTime);
	Position calcPosition(DWORD currentTime);
	void setBallData(const Position &start, const Position &end, float maxY, DWORD startTime, DWORD endTime);
};

struct Data
{
	BallData ball;
	std::vector<Player *> defencer;

	float ballSpeed;
	DWORD hitTime;
	DWORD startTime;

	void setDefencerTarget(DWORD currentTime, const Position &target);
	void stopDefencer(DWORD currentTime);
};

#endif