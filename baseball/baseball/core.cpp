#include <gl/glew.h>
#include <gl/glut.h>
#include "game.h"
#include "util.h"
#include "input.h"
#include "draw.h"
#include "core.h"

#define NEAR_LIMIT 0.3f
#define LAND_LIMIT 0.01f
#define STRIKE_ZONE_Y 1.0f
#define HIT_LIMIT 1.3f

float random(float a, float b)
{
	double t = (double)rand() / (RAND_MAX + 1);
	return float((b - a) * t + a);
}

void display()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 100000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DWORD currentTime = getTime();

	if (game->getState() == Game::CATCHED || game->getState() == Game::FINISH_FLYING)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScore();
	}
	else
	{
		glClearColor(0.46, 0.80, 0.88, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (game->getState() == Game::FLYING)
		{
			float cameraHeight = 30;
			Position p(data->ball.getPosition(getTime()));
			gluLookAt(p.x, cameraHeight, p.z, p.x, 0, p.z, 0, 0, -1);
		}
		else
		{
			gluLookAt(0, 13, -FIELD / 2, 0, 0, -FIELD / 2, 0, 0, -1);
		}

		drawMap();
		drawUnit(currentTime);
		drawBall(currentTime);
	}

	glutSwapBuffers();
	glFlush();
}

void initGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutCreateWindow("BaseBall");
	glewInit();

	glutDisplayFunc(display);
}

void hideBall()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	float BELOW = -10;
	data->ball.setBallData(Position(0, BELOW, 0), Position(0, BELOW, 0), BELOW, 0, 0);
}

void setThrowFactor()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	Unit *unit = game->getUnit();

	DWORD currentTime = getTime();
	Position pitcher = unit->getPosition(Unit::PITCHER, currentTime);
	Position catcher = unit->getPosition(Unit::CATCHER, currentTime);
	
	pitcher.y = STRIKE_ZONE_Y;
	catcher.y = STRIKE_ZONE_Y;

	DWORD throwTime = (DWORD)(FIELD / convertKMPHToMPMS(data->ballSpeed));
	data->hitTime = random(currentTime + throwTime * 0.1f, currentTime + throwTime);
	data->ball.setBallData(pitcher, catcher, STRIKE_ZONE_Y + 0.1f, currentTime, currentTime + throwTime);
}

void setHitFactor(float ballSpeed)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	Unit *unit = game->getUnit();

	float angle = random(PI / 4, 3 * PI / 4);
	float len = random(15 + ballSpeed * 0.15f, ballSpeed + 10);

	DWORD currentTime = getTime();
	Position start(0, STRIKE_ZONE_Y, 0);
	Position end(len * cos(angle), 0, len * -sin(angle));
	float maxY = random(3 + (ballSpeed - 120) / 10, 10 + (ballSpeed - 120) / 10);

	unit->calcNearestDefencer(end);
	data->setDefencerTarget(currentTime, end);
	data->ball.setBallData(start, end, maxY, currentTime, currentTime + (DWORD)(calcHorizontalDist(start, end) / convertKMPHToMPMS(data->ballSpeed)));
}

void setRollFactor()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	Unit *unit = game->getUnit();
	
	DWORD currentTime = getTime();
	Position ballStart = data->ball.getStartPosition();
	Position ballEnd = data->ball.getEndPosition();

	float dx = ballEnd.x - ballStart.x;
	float dz = ballEnd.z - ballStart.z;
	float reduce = 4;

	data->ballSpeed /= reduce;
	Position start(data->ball.getPosition(currentTime));
	Position end(ballEnd.x + dx / reduce, 0, ballEnd.z + dz / reduce);

	data->ball.setBallData(start, end, 0, currentTime, currentTime + (DWORD)(calcHorizontalDist(start, end) / convertKMPHToMPMS(data->ballSpeed)));
}

void advance()
{
	Game *game = Game::getInstance();
	Unit *unit = game->getUnit();
	Score *score = game->getScore();

	Position base[4] = {
		Position(0, 0, 0),
		Position(FIELD, 0, -FIELD),
		Position(0, 0, -2 * FIELD),
		Position(-FIELD, 0, -FIELD),
	};

	DWORD currentTime = getTime();
	DWORD advanceTime = (DWORD)(sqrt(2.0f) * FIELD / PLAYER_SPEED);

	float BELOW = -10;
	unit->getPlayer(Unit::BATTER)->setData(Position(0, BELOW, 0), Position(0, BELOW, 0), BELOW, 0, 0);
	unit->getPlayer(Unit::RUNNER1)->setData(base[0], base[1], 0, currentTime, currentTime + advanceTime);
	unit->getPlayer(Unit::RUNNER2)->setData(base[1], base[2], 0, currentTime, currentTime + advanceTime);
	unit->getPlayer(Unit::RUNNER3)->setData(base[2], base[3], 0, currentTime, currentTime + advanceTime);
	unit->getPlayer(Unit::RUNNER4)->setData(base[3], base[0], 0, currentTime, currentTime + advanceTime);

	for (int i = 4; 1 < i; --i)
	{
		score->setRunnerOn(i, score->isRunnerOn(i - 1));
	}

	score->setRunnerOn(1, true);
}

void waitForThrowing()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	Input *input = Input::getInstance();
	Unit *unit = game->getUnit();
	
	int value = abs(input->getValue());
	if (1 <= value)
	{
		Pitcher *pitcher = (Pitcher *)unit->getPlayer(Unit::PITCHER);
		pitcher->setPitchTime(getTime());

		data->ballSpeed = 90 + (value - 1) * 20;
		game->nextState(Game::THROW_ANIMATION, throwAnimation);
	}

	glutPostRedisplay();
}

void throwAnimation()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	if (data->startTime + THROW_TIME < getTime())
	{
		setThrowFactor();
		game->nextState(Game::ON_THROW, throwSimulation);
	}

	glutPostRedisplay();
}

bool swing(DWORD currentTime, float speed)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();
	Score *score = game->getScore();
	Unit *unit = game->getUnit();
	Position ball = data->ball.getPosition(currentTime);

	Batter *batter = (Batter *)unit->getPlayer(Unit::BATTER);
	batter->setBatTime(currentTime);

	if (calcDist(ball, Position(0, STRIKE_ZONE_Y, 0)) < HIT_LIMIT)
	{
		advance();
		score->hit();
		setHitFactor(speed);
		game->nextState(Game::FLYING, simulateFlying);
		return true;
	}

	return false;
}

void throwSimulation()
{
	static DWORD lastTime = 0;
	static bool isSwinged = false;

	Game *game = Game::getInstance();
	Data *data = game->getData();
	Unit *unit = game->getUnit();
	Score *score = game->getScore();

	if (lastTime < data->startTime)
	{
		lastTime = data->startTime;
		isSwinged = false;
	}

	DWORD currentTime = getTime();
	Position ball = data->ball.getPosition(currentTime);
	Position catcher = unit->getPosition(Unit::CATCHER, currentTime);
	catcher.y = STRIKE_ZONE_Y;

	if (!isSwinged)
	{
		if (game->isAttackMode())
		{
			Input *input = Input::getInstance();
			float value = abs(input->getX());

			if (1 <= value)
			{
				isSwinged = true;
				if(swing(currentTime, 120 + 50 * (value - 1))) return;
			}
		}
		else
		{
			if (data->hitTime < currentTime)
			{
				isSwinged = true;
				if (swing(currentTime, 120)) return;
			}
		}
	}

	if (calcDist(ball, catcher) < NEAR_LIMIT)
	{
		score->strike();
		hideBall();
		game->nextState(Game::CATCHED, catchAnimation);
	}

	glutPostRedisplay();
}

void simulateFlying()
{
	static DWORD lastTime = 0;
	static bool isRolled = false;

	DWORD currentTime = getTime();
	Game *game = Game::getInstance();
	Score *score = game->getScore();
	Data *data = game->getData();
	Unit *unit = game->getUnit();

	if (lastTime < data->startTime)
	{
		lastTime = data->startTime;
		isRolled = false;
	}

	Position ball(data->ball.getPosition(currentTime));

	if ((STADIUM_RADIUS + 10) * (STADIUM_RADIUS + 10) <= ball.x * ball.x + ball.z * ball.z)
	{
		score->homeRun();
		game->nextState(Game::FINISH_FLYING, onFinishFlying);
		return;
	}

	bool isCatched = false;
	for (size_t i = 0; i < data->defencer.size(); ++i)
	{
		if (calcDist(data->defencer[i]->getPosition(currentTime), ball) < NEAR_LIMIT)
		{
			isCatched = true;
		}
	}

	if (isCatched)
	{
		if (!isRolled)
		{
			for (int i = 1; i < 4; ++i)
			{
				score->setRunnerOn(i, score->isRunnerOn(i + 1));
			}

			score->setRunnerOn(4, false);
			score->out();
		}

		if (score->isRunnerOn(4)) score->makeScore();
		game->nextState(Game::FINISH_FLYING, onFinishFlying);
		return;
	}
	else if (!isRolled && ball.y < LAND_LIMIT)
	{
		if (STADIUM_RADIUS * STADIUM_RADIUS <= ball.x * ball.x + ball.z * ball.z)
		{
				score->homeRun();
				game->nextState(Game::FINISH_FLYING, onFinishFlying);
				return;
		}

		setRollFactor();
		isRolled = true;
	}

	if (isRolled)
	{
		data->setDefencerTarget(currentTime, data->ball.getPosition(currentTime));
	}
	else
	{
		for (size_t i = 0; i < data->defencer.size(); ++i)
		{
			if (data->defencer[i]->isArrived(currentTime))
			{
				data->stopDefencer(currentTime);
				break;
			}
		}
	}

	glutPostRedisplay();
}

void catchAnimation()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	if (data->startTime + 2000 < getTime())
	{
		game->nextState(Game::MAIN, mainLoop);
	}

	glutPostRedisplay();
}

void onFinishFlying()
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	if (data->startTime + 2000 < getTime())
	{
		Game::getInstance()->nextState(Game::MAIN, mainLoop);
	}

	glutPostRedisplay();
}

void mainLoop()
{
	Game *game = Game::getInstance();
	Score *score = game->getScore();
	Unit *unit = game->getUnit();
	Data *data = game->getData();
	bool isFirstHalf = game->isFirstHalf();
	
	switch (game->getState())
	{

	case Game::INIT:
		game->init();
		game->nextState(Game::MAIN, mainLoop);
		break;

	case Game::MAIN:
		score->reset();

		if (score->getResult() == Score::THREE_OUT)
		{
			score->changeRotation();
			game->toggleAttackMode();

			if (!isFirstHalf)
			{
				int inning = game->getInning();

				if (Game::MAX_INNING <= inning && score->getTotalScore(Score::USER) != score->getTotalScore(Score::ENEMY)) break;
				else if (inning < Game::MAX_EXTENSION_INNING) game->nextInning();
				else
				{
					game->nextState(Game::FINISH, NULL);
					break;
				}
			}
		}

		hideBall();
		unit->initUnit();

		if (game->isAttackMode())
		{
			Pitcher *pitcher = (Pitcher *)unit->getPlayer(Unit::PITCHER);
			pitcher->setPitchTime(getTime());
			data->ballSpeed = random(90, 110);
			game->nextState(Game::THROW_ANIMATION, throwAnimation);
		}
		else
		{
			game->nextState(Game::WAIT_FOR_THROW, waitForThrowing);
		}

		game->toggleFirstHalf();
		break;
	}
}
