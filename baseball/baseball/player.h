#ifndef _PLAYER_H
#define _PLAYER_H

#include <Windows.h>
#include "util.h"

class Player
{

public:

	enum State
	{
		NORMAL
	};

protected:

	Position m_start;
	Position m_end;
	DWORD m_startTime;
	DWORD m_endTime;
	DWORD m_maxY;

	bool m_isAttacker;
	State m_state;

public:

	Player() : m_isAttacker(false), m_state(NORMAL) {}
	virtual void draw(DWORD currentTime);
	void setToAttacker() { m_isAttacker = true; }
	void setState(State state) { m_state = state; }

	Position getStartPosition() { return m_start; }
	Position getEndPosition() { return m_end; }
	Position getPosition(DWORD time);

	void setPosition(const Position &p);
	void setData(const Position &start, const Position &end, float maxY, DWORD startTime, DWORD endTime);
	bool isArrived(DWORD currentTime) { return m_endTime <= currentTime; }
};

class Pitcher : public Player
{

private:

	DWORD m_pitchTime;

public:

	void setPitchTime(DWORD currentTime) { m_pitchTime = currentTime; }
	virtual void draw(DWORD currentTime);

};

class Catcher : public Player
{

public:

	virtual void draw(DWORD currentTime);

};

class Batter : public Player
{

private:

	DWORD m_batTime;

public:

	void setBatTime(DWORD currentTime) { m_batTime = currentTime; }
	virtual void draw(DWORD currentTime);

};

#endif