#include <gl/glut.h>
#include "game.h"
#include "score.h"
#include "unit.h"
#include "core.h"
#include "draw.h"
#include "Hop.h"

Game Game::s_instance;

Game::Game()
{

}

void Game::init()
{
	bool isUserFirst = true;

	m_inning = 1;
	m_isFirstHalf = true;
	m_isAttackMode = isUserFirst;
	m_score.init(isUserFirst);
}

void Game::play()
{
	Game::getInstance()->nextState(Game::INIT, mainLoop);
}

void Game::nextState(GameState nextState, void (*func)())
{
	m_state = nextState;
	m_data.startTime = getTime();
	glutIdleFunc(func);
}
