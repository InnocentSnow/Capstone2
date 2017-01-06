#ifndef _GAME_H
#define _GAME_H

#include "data.h"
#include "score.h"
#include "unit.h"

class Game
{

public:

	enum
	{
		MAX_INNING = 9,
		MAX_EXTENSION_INNING = 12
	};

	enum GameState
	{
		INIT,
		MAIN,
		FINISH,

		WAIT_FOR_THROW,
		THROW_ANIMATION,
		ON_THROW,
		CATCHED,
		FLYING,
		FINISH_FLYING
	};

private:

	Data m_data;
	GameState m_state;

	bool m_isFirstHalf;
	bool m_isAttackMode;
	Score m_score;
	Unit m_unit;

	int m_inning;

	void attackPlay();
	void defencePlay();

	Game();
	static Game s_instance;

public:

	void init();
	void play();
	void nextState(GameState nextState, void(*func)());

	void toggleAttackMode() { m_isAttackMode = !m_isAttackMode; }
	bool isAttackMode() { return m_isAttackMode;}

	void toggleFirstHalf() { m_isFirstHalf = !m_isFirstHalf; }
	bool isFirstHalf() { return m_isFirstHalf; }

	int getInning() { return m_inning; }
	void nextInning() { ++m_inning; }

	GameState getState() { return m_state; }
	Unit *getUnit() { return &m_unit; }
	Data *getData() { return &m_data; }
	Score *getScore() { return &m_score; }

	static Game *getInstance() { return &s_instance; }
};

#endif