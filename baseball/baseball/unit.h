#ifndef _UNIT_H
#define _UNIT_H

#include <vector>
#include "player.h"
#include "util.h"

class Unit
{

public:

	enum PlayerType
	{
		PITCHER,
		CATCHER,
		SHORT_STOP,
		FIRST_BASE,
		SECOND_BASE,
		THIRD_BASE,
		LEFT_FIELDER,
		RIGHT_FIELDER,
		CENTER_FIELDER,

		BATTER,
		RUNNER1,
		RUNNER2,
		RUNNER3,
		RUNNER4,

		NUM_PLAYER
	};

private:

	Player *m_player[NUM_PLAYER];
	std::vector<Player *> m_defencer;

public:

	Unit();
	~Unit();

	void draw(DWORD currentTime);
	void initUnit();
	Position getPosition(PlayerType id, DWORD time);
	void calcNearestDefencer(const Position &p);
	Player *getPlayer(PlayerType id) { return m_player[id]; }
};

#endif
