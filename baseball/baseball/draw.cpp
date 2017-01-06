#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <gl/glut.h>
#include "game.h"
#include "player.h"
#include "data.h"
#include "Hop.h"
#include "util.h"
#include "text.h"
#include "drawutil.h"
#include "texture.h"

void drawTextCenter(const char *str, float x1, float x2, float y1, float y2, float size)
{
	int l = strlen(str);

	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (100 < x2) x2 = 100;
	if (100 < y2) y2 = 100;

	if (x2 - x1 < l * size) size = (x2 - x1) / l;
	Text::getInstance()->draw(str, (x1 + x2 - l * size) / 2, (y1 + y2 - size) / 2, size);
}

void drawTextCenter(const char *str, float x1, float x2, float y1, float y2)
{
	int l = strlen(str);

	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	if (100 < x2) x2 = 100;
	if (100 < y2) y2 = 100;

	float size = std::fminf(y2 - y1, (x2 - x1) / l);
	Text::getInstance()->draw(str, (x1 + x2 - l * size) / 2, (y1 + y2 - size) / 2, size);
}

void drawTextCenter(const char *str, float y, float size)
{
	int l = strlen(str);

	if (100 < l * size) size = 100.0f / l;
	Text::getInstance()->draw(str, (100 - l * size) / 2, y, size);
}

void drawScore()
{
	Game *game = Game::getInstance();
	Score *score = game->getScore();
	Text *text = Text::getInstance();

	glDepthFunc(GL_ALWAYS);
	glOrtho(0, 100, 100, 0, 0.1, 10000);

	char buf[128];
	float size = 100.0f / (Game::MAX_EXTENSION_INNING + 4);
	float margin = 0.5f;
	float lineMargin = 0.3f;
	float y = 100 - 3 * size - lineMargin;

	float s = 5.0f;
	glColor3f(0, 0.7f, 1);
	sprintf_s(buf, "%d BALL  ", score->getBall());
	drawTextCenter(buf, 100 - s * (strlen(buf) + 1), 100, y - 4 * s, y - 3 * s, s);
	sprintf_s(buf, "%d STRIKE", score->getStrike());
	drawTextCenter(buf, 100 - s * (strlen(buf) + 1), 100, y - 3 * s, y - 2 * s, s);
	sprintf_s(buf, "%d OUT   ", score->getOut());
	drawTextCenter(buf, 100 - s * (strlen(buf) + 1), 100, y - 2 * s, y - s, s);

	glPushMatrix();
	glTranslatef(2 * s, y - 4 * s, 0);

	glLineWidth(3.0f);
	float X[] = { 2 * s, s, 0, s };
	float Y[] = { s, 0, s, 2 * s };

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			glPushMatrix();
			glTranslatef(X[i], Y[i], 0);

			if (j == 0)
			{
				if (!score->isRunnerOn(i + 1))
				{
					glPopMatrix();
					continue;
				}

				glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
			}
			else
			{
				glBegin(GL_LINE_LOOP);
				glColor3f(1, 1, 1);
			}

			glVertex3f(0, s / 2, 0);
			glVertex3f(s / 2, 0, 0);
			glVertex3f(s, s / 2, 0);
			glVertex3f(s / 2, s, 0);
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();

	int inning = score->getInning();
	Score::UserType first = score->isUserFirst() ? Score::USER : Score::ENEMY;
	Score::UserType second = score->isUserFirst() ? Score::ENEMY : Score::USER;

	for (int i = 1; i <= Game::MAX_EXTENSION_INNING; ++i)
	{
		glColor3f(1, 1, 1);
		sprintf_s(buf, "%d", i);
		drawTextCenter(buf, (i + 1) * size + margin, (i + 2) * size - margin, y, y + size);

		if (i <= inning)
		{
			setColor(first);
			sprintf_s(buf, "%d", score->getScore(first, i));
			drawTextCenter(buf, (i + 1) * size + margin, (i + 2) * size - margin, y + size, y + 2 * size);
		}

		if (i < inning || (inning == i && score->getTurn()))
		{
			setColor(second);
			sprintf_s(buf, "%d", score->getScore(second, i));
			drawTextCenter(buf, (i + 1) * size + margin, (i + 2) * size - margin, y + 2 * size, y + 3 * size);
		}
	}

	setColor(first);
	sprintf_s(buf, "%d", score->getTotalScore(first));
	drawTextCenter(first == Score::USER ? "USER" : "ENEMY", margin, 2 * size - margin, y + size, y + 2 * size);
	drawTextCenter(buf, (Game::MAX_EXTENSION_INNING + 2) * size + margin, (Game::MAX_EXTENSION_INNING + 4) * size - margin, y + size, y + 2 * size);

	setColor(second);
	sprintf_s(buf, "%d", score->getTotalScore(second));
	drawTextCenter(second == Score::USER ? "USER" : "ENEMY", margin, 2 * size - margin, y + 2 * size, y + 3 * size);
	drawTextCenter(buf, (Game::MAX_EXTENSION_INNING + 2) * size + margin, (Game::MAX_EXTENSION_INNING + 4) * size - margin, y + 2 * size, y + 3 * size);

	glColor3f(1, 1, 1);
	drawTextCenter("TEAM", margin, 2 * size - margin, y, y + size);
	drawTextCenter("TOTAL", (Game::MAX_EXTENSION_INNING + 2) * size + margin, (Game::MAX_EXTENSION_INNING + 4) * size - margin, y, y + size);

	glLineWidth(5.0f);

	glBegin(GL_LINES);
	for (int i = 0; i < 4; ++i)
	{
		glVertex3f(0, y + i * size, 0);
		glVertex3f(100, y + i * size, 0);
	}

	glVertex3f(lineMargin, y, 0);
	glVertex3f(lineMargin, y + 3 * size, 0);
	glVertex3f(100 - lineMargin, y, 0);
	glVertex3f(100 - lineMargin, y + 3 * size, 0);

	for (int i = 2; i <= Game::MAX_EXTENSION_INNING + 2; ++i)
	{
		glVertex3f(i * size, y, 0);
		glVertex3f(i * size, y + 3 * size, 0);
	}
	glEnd();

	drawTextCenter(score->getResultString().c_str(), 30, 10);
	glDepthFunc(GL_LESS);
}

void drawLine()
{
	glDepthFunc(GL_ALWAYS);
	const float STADIUM = STADIUM_RADIUS / sqrt(2.0f);
	const float breadth = 0.1f;
	const float lineBreadth = breadth / sqrt(2.0f);

	glColor3f(1, 1, 1);
	for (int j = 0; j < 2; ++j)
	{
		float r = j ? -1 : 1;

		glBegin(GL_POLYGON);
		glVertex3f(r * -lineBreadth, 0, -lineBreadth);
		glVertex3f(r * (-lineBreadth + STADIUM), 0, -lineBreadth - STADIUM);
		glVertex3f(r * (lineBreadth + STADIUM), 0, lineBreadth - STADIUM);
		glVertex3f(r * lineBreadth, 0, lineBreadth);
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(r * STADIUM, 0, -STADIUM);
		for (int i = 0; i <= NUM_CIRCLE_POINT; ++i)
		{
			float theta = PI / 4 - (PI / 2) * i / NUM_CIRCLE_POINT;
			glVertex3f(r * (STADIUM + breadth * cos(theta)), 0, -STADIUM + breadth * sin(theta));
		}
		glEnd();
	}

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= NUM_CIRCLE_POINT; ++i)
	{
		float theta = -PI / 4 - (PI / 2) * i / NUM_CIRCLE_POINT;
		glVertex3f((STADIUM_RADIUS + breadth) * cos(theta), 0, (STADIUM_RADIUS + breadth) * sin(theta));
		glVertex3f((STADIUM_RADIUS - breadth) * cos(theta), 0, (STADIUM_RADIUS - breadth) * sin(theta));
	}
	glEnd();
	glDepthFunc(GL_LESS);
}

void drawMap()
{
	Texture *texture = Texture::getInstance();
	float sizeX = 102.4f;
	float sizeZ = 120.0f;

	texture->useTexture(Texture::MAP);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-sizeX, 0, -sizeZ);
	glTexCoord2f(1, 1);
	glVertex3f(sizeX, 0, -sizeZ);
	glTexCoord2f(1, 0);
	glVertex3f(sizeX, 0, sizeZ);
	glTexCoord2f(0, 0);
	glVertex3f(-sizeX, 0, sizeZ);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	drawLine();

	glEnable(GL_CULL_FACE);
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor3f(1, 1, 1);
	const float fenceLen = STADIUM_RADIUS + 0.65f;
	for (int i = 0; i <= NUM_CIRCLE_POINT; ++i)
	{
		float theta = -PI / 4 - (PI / 2) * i / NUM_CIRCLE_POINT;
		glVertex3f(fenceLen * cos(theta), 0, fenceLen * sin(theta));
		glVertex3f(fenceLen * cos(theta), FENCE_HEIGHT, fenceLen * sin(theta));
	}
	glEnd();

	glDisable(GL_CULL_FACE);
}

void drawBall(DWORD currentTime)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	Position p(data->ball.getPosition(currentTime));
	p.y += BALL_SIZE;

	glColor3f(1, 0, 0);
	drawSphere(p, BALL_SIZE);
}

void drawUnit(DWORD currentTime)
{
	Game *game = Game::getInstance();
	Unit *unit = game->getUnit();
	unit->draw(currentTime);
}
