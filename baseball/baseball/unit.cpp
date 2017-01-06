#include <gl/glut.h>
#include "unit.h"
#include "util.h"
#include "game.h"

Unit::Unit()
{
	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		switch (i)
		{

		case PlayerType::PITCHER:
			m_player[i] = new Pitcher();
			break;

		case PlayerType::CATCHER:
			m_player[i] = new Catcher();
			break;

		case PlayerType::BATTER:
			m_player[i] = new Batter();
			break;

		default:
			m_player[i] = new Player();
		}
	}

	m_defencer.push_back(m_player[CATCHER]);
	m_defencer.push_back(m_player[PITCHER]);
	m_defencer.push_back(m_player[SHORT_STOP]);

	m_defencer.push_back(m_player[LEFT_FIELDER]);
	m_defencer.push_back(m_player[RIGHT_FIELDER]);
	m_defencer.push_back(m_player[CENTER_FIELDER]);

	m_player[BATTER]->setToAttacker();
	m_player[RUNNER1]->setToAttacker();
	m_player[RUNNER2]->setToAttacker();
	m_player[RUNNER3]->setToAttacker();
	m_player[RUNNER4]->setToAttacker();
}

Unit::~Unit()
{
	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		delete m_player[i];
	}
}

void Unit::initUnit()
{
	m_player[PITCHER]->setPosition(Position(0, 0, -FIELD));
	m_player[CATCHER]->setPosition(Position(0, 0, 2));
	m_player[SHORT_STOP]->setPosition(Position(-10, 0, -25));

	m_player[FIRST_BASE]->setPosition(Position(FIELD, 0, -FIELD - 1));
	m_player[SECOND_BASE]->setPosition(Position(0, 0, -2 * FIELD - 1));
	m_player[THIRD_BASE]->setPosition(Position(-FIELD, 0, -FIELD - 1));

	m_player[LEFT_FIELDER]->setPosition(Position(-20, 0, -50));
	m_player[RIGHT_FIELDER]->setPosition(Position(20, 0, -50));
	m_player[CENTER_FIELDER]->setPosition(Position(0, 0, -60));

	m_player[BATTER]->setPosition(Position(-1, 0, 0));

	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		m_player[i]->setState(Player::NORMAL);
	}
}

void Unit::draw(DWORD currentTime)
{
	Game *game = Game::getInstance();
	Score *score = game->getScore();
	Data *data = game->getData();

	for (int i = 0; i < NUM_PLAYER; ++i)
	{
		if (i == Unit::RUNNER1 && !score->isRunnerOn(1)) continue;
		if (i == Unit::RUNNER2 && !score->isRunnerOn(2)) continue;
		if (i == Unit::RUNNER3 && !score->isRunnerOn(3)) continue;
		if (i == Unit::RUNNER4 && !score->isRunnerOn(4)) continue;

		DWORD currentTime = getTime();
		Position p = m_player[i]->getPosition(currentTime);
		Position ball = data->ball.getPosition(currentTime);

		glPushMatrix();
		glTranslatef(p.x, p.y, p.z);

		if (i == Unit::RUNNER1) glRotatef(135, 0, 1, 0);
		else if (i == Unit::RUNNER2) glRotatef(225, 0, 1, 0);
		else if (i == Unit::RUNNER3) glRotatef(315, 0, 1, 0);
		else if (i == Unit::RUNNER4) glRotatef(45, 0, 1, 0);
		else if (i == Unit::BATTER) glRotatef(135, 0, 1, 0);
		else
		{
			Position target;
			bool isDefencer = false;

			for (size_t j = 0; j < data->defencer.size(); ++j)
			{
				if (data->defencer[j] == m_player[i])
				{
					isDefencer = true;
				}
			}

			if (game->getState() == Game::FLYING && isDefencer && !m_player[i]->isArrived(currentTime))
			{
				target = m_player[i]->getEndPosition();
			}
			else
			{
				target = ball;
			}

			float dx = target.x - p.x;
			float dz = target.z - p.z;
			float l = calcHorizontalDist(p, target);

			glRotatef(acosf(dz / l) * 180 / PI, 0, 0 < dx ? 1 : -1, 0);
		}

		m_player[i]->draw(currentTime);
		glPopMatrix();
	}
}

Position Unit::getPosition(PlayerType id, DWORD time)
{
	return m_player[id]->getPosition(time);
}

void Unit::calcNearestDefencer(const Position &p)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	DWORD currentTime = getTime();

	float dist[NUM_PLAYER];
	float minDist = STADIUM_RADIUS;
	
	for (size_t i = 0; i < m_defencer.size(); ++i)
	{
		dist[i] = calcHorizontalDist(p, m_defencer[i]->getPosition(currentTime));
		
		if (dist[i] < minDist)
		{
			minDist = dist[i];
		}
	}

	data->defencer.clear();
	for (size_t i = 0; i < m_defencer.size(); ++i)
	{
		if (dist[i] <= minDist + 20)
		{
			data->defencer.push_back(m_defencer[i]);
		}
	}
}
