#include <Windows.h>
#include <gl/glut.h>
#include "player.h"
#include "game.h"
#include "drawutil.h"
#include "util.h"

const float HEIGHT = 1.7f;
const float NECK_TOP = 1.6f;
const float NECK_BOTTOM = 1.45f;
const float BODY_TOP = 1.4f;
const float ARM_Y = 1.37f;
const float ARM_X = 0.17f;
const float HAND_Y = 0.7f;
const float HAND_X = 0.25f;
const float LEG_Y = 0.9f;
const float LEG_X = 0.1f;
const float FOOT_X = 0.1f;
const float FACE_SIZE = 0.15f;
const float PERIOD = 500;
const float LEG_ANGLE = 30;
const float ARM_ANGLE = 20;

Position Player::getPosition(DWORD currentTime)
{
	if (m_endTime < currentTime) currentTime = m_endTime;
	float portion = getPortion(m_startTime, m_endTime, currentTime);

	float x = linearInterpolate(m_start.x, m_end.x, portion);
	float z = linearInterpolate(m_start.z, m_end.z, portion);
	float y;

	DWORD halfTime = (m_startTime + m_endTime) / 2;
	if (currentTime < halfTime) y = squareInterpolate(0.0f, m_maxY - m_start.y, getPortion(m_startTime, halfTime, currentTime)) + m_start.y;
	else y = squareInterpolate(m_maxY - m_end.y, 0.0f, getPortion(halfTime, m_endTime, currentTime)) + m_end.y;

	return Position(x, y, z);
}

void Player::setPosition(const Position &p)
{
	setData(p, p, p.y, 0, 0);
}

void Player::setData(const Position &start, const Position &end, float maxY, DWORD startTime, DWORD endTime)
{
	m_start = start;
	m_end = end;
	m_maxY = maxY;

	m_startTime = startTime;
	m_endTime = endTime;
}

void Player::draw(DWORD currentTime)
{
	Game *game = Game::getInstance();

	if ((m_isAttacker && game->isAttackMode()) || (!m_isAttacker && !game->isAttackMode())) setColor(Score::USER);
	else setColor(Score::ENEMY);

	drawSphere(Position(0, HEIGHT, 0), FACE_SIZE);
	drawCylinder(Position(0, NECK_TOP, 0), Position(0, NECK_BOTTOM, 0), 0.05f, 0.05f);

	drawBody(NECK_BOTTOM, BODY_TOP, 0.05f, 0.05f, 0.2f, 0.05f);
	drawBody(BODY_TOP, LEG_Y, 0.2f, 0.05f, 0.15f, 0.05f);

	float leftArm, rightArm, leftLeg, rightLeg;

	if (isArrived(currentTime))
	{
		leftArm = 0;
		rightArm = 0;
		leftLeg = 0;
		rightLeg = 0;
	}
	else
	{
		leftArm = ARM_ANGLE * sin(currentTime * (2 * PI / PERIOD));
		rightArm = ARM_ANGLE * sin(PI + currentTime * (2 * PI / PERIOD));

		leftLeg = LEG_ANGLE * sin(PI + currentTime * (2 * PI / PERIOD));
		rightLeg = LEG_ANGLE * sin(currentTime * (2 * PI / PERIOD));

		if (leftArm < 0) leftArm *= 0.3f;
		if (rightArm < 0) rightArm *= 0.3f;

		if (leftLeg < 0) leftLeg = 0;
		if (rightLeg < 0) rightLeg = 0;
	}

	glPushMatrix();
	glTranslatef(-LEG_X, LEG_Y, 0);
	glRotatef(rightLeg, -1, 0, 0);
	drawCylinder(Position(0, 0, 0), Position(-FOOT_X + LEG_X, -LEG_Y, 0), 0.05f, 0.03f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(LEG_X, LEG_Y, 0);
	glRotatef(leftLeg, -1, 0, 0);
	drawCylinder(Position(0, 0, 0), Position(FOOT_X - LEG_X, -LEG_Y, 0), 0.05f, 0.03f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-ARM_X, ARM_Y, 0);
	glRotatef(rightArm, -1, 0, 0);
	drawCylinder(Position(0, 0, 0), Position(-HAND_X + ARM_X, HAND_Y - ARM_Y, 0), 0.03f, 0.02f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(ARM_X, ARM_Y, 0);
	glRotatef(leftArm, -1, 0, 0);
	drawCylinder(Position(0, 0, 0), Position(HAND_X - ARM_X, HAND_Y - ARM_Y, 0), 0.03f, 0.02f);
	glPopMatrix();
}

void Pitcher::draw(DWORD currentTime)
{
	if (!isArrived(currentTime))
	{
		Player::draw(currentTime);
		return;
	}

	Game *game = Game::getInstance();

	if ((m_isAttacker && game->isAttackMode()) || (!m_isAttacker && !game->isAttackMode())) setColor(Score::USER);
	else setColor(Score::ENEMY);

	drawSphere(Position(0, HEIGHT, 0), FACE_SIZE);
	drawCylinder(Position(0, NECK_TOP, 0), Position(0, NECK_BOTTOM, 0), 0.05f, 0.05f);

	drawBody(NECK_BOTTOM, BODY_TOP, 0.05f, 0.05f, 0.2f, 0.05f);
	drawBody(BODY_TOP, LEG_Y, 0.2f, 0.05f, 0.15f, 0.05f);

	const DWORD SWING = 300;
	const DWORD DOWN = 500;
	const float MAX_ANGLE = 200.0f;

	float rightArm = 0;
	if (currentTime < m_pitchTime + THROW_TIME - SWING) rightArm = MAX_ANGLE * (currentTime - m_pitchTime) / (THROW_TIME - SWING);
	else if (currentTime < m_pitchTime + THROW_TIME) rightArm = MAX_ANGLE - MAX_ANGLE * (currentTime - (m_pitchTime + THROW_TIME - SWING)) / SWING;

	float armLen = (HAND_Y - ARM_Y) * (0.7f + 0.3f * (1 - rightArm / 180));

	glPushMatrix();
	glTranslatef(-ARM_X, ARM_Y, 0);
	glRotatef(rightArm, -1, 0, 0);
	drawCylinder(Position(0, 0, 0), Position(-HAND_X * (1 - rightArm / 180) + ARM_X, armLen, 0), 0.03f, 0.02f);
	glPopMatrix();

	drawCylinder(Position(-LEG_X, LEG_Y, 0), Position(-FOOT_X, 0, 0), 0.05f, 0.03f);
	drawCylinder(Position(LEG_X, LEG_Y, 0), Position(FOOT_X, 0, 0), 0.05f, 0.03f);
	drawCylinder(Position(ARM_X, ARM_Y, 0), Position(HAND_X, HAND_Y, 0), 0.03f, 0.02f);
}

void Catcher::draw(DWORD currentTime)
{
	if (!isArrived(currentTime))
	{
		Player::draw(currentTime);
		return;
	}

	Game *game = Game::getInstance();

	if ((m_isAttacker && game->isAttackMode()) || (!m_isAttacker && !game->isAttackMode())) setColor(Score::USER);
	else setColor(Score::ENEMY);

	float sit = LEG_Y / 2;

	drawSphere(Position(0, HEIGHT - sit, 0), FACE_SIZE);
	drawCylinder(Position(0, NECK_TOP - sit, 0), Position(0, NECK_BOTTOM - sit, 0), 0.05f, 0.05f);

	drawBody(NECK_BOTTOM - sit, BODY_TOP - sit, 0.05f, 0.05f, 0.2f, 0.05f);
	drawBody(BODY_TOP - sit, LEG_Y - sit, 0.2f, 0.05f, 0.15f, 0.05f);

	{
		float HAND_X = ARM_X + 0.1f;
		float HAND_Y = ARM_Y + 0.2f;
		float z = 0.5f;

		drawCylinder(Position(-ARM_X, ARM_Y - sit, 0), Position(-HAND_X, HAND_Y - sit, z), 0.03f, 0.02f);
		drawCylinder(Position(ARM_X, ARM_Y - sit, 0), Position(HAND_X, HAND_Y - sit, z), 0.03f, 0.02f);
	
		float dy = 0.3f;
		float dz = 0.3f;

		drawCylinder(Position(-LEG_X, LEG_Y - sit, 0), Position(-LEG_X, LEG_Y - sit + dy, dz), 0.05f, 0.04f);
		drawCylinder(Position(-LEG_X, LEG_Y - sit + dy, dz), Position(-LEG_X, 0, 2 * dz), 0.04f, 0.03f);

		drawCylinder(Position(LEG_X, LEG_Y - sit, 0), Position(LEG_X, LEG_Y - sit + dy, dz), 0.05f, 0.03f);
		drawCylinder(Position(LEG_X, LEG_Y - sit + dy, dz), Position(LEG_X, 0, 2 * dz), 0.05f, 0.03f);
	}
}

void Batter::draw(DWORD currentTime)
{
	Game *game = Game::getInstance();

	if ((m_isAttacker && game->isAttackMode()) || (!m_isAttacker && !game->isAttackMode())) setColor(Score::USER);
	else setColor(Score::ENEMY);

	const DWORD BAT_TIME = 100;
	bool swing = currentTime < m_batTime + BAT_TIME;

	if (swing)
	{
		float t = (float)(currentTime - m_batTime) / BAT_TIME;
		float angle = 60 * t - 45;

		glPushMatrix();
		glRotatef(angle, 0, 1, 0);
	}

	drawSphere(Position(0, HEIGHT, 0), FACE_SIZE);
	drawCylinder(Position(0, NECK_TOP, 0), Position(0, NECK_BOTTOM, 0), 0.05f, 0.05f);

	drawBody(NECK_BOTTOM, BODY_TOP, 0.05f, 0.05f, 0.2f, 0.05f);
	drawBody(BODY_TOP, LEG_Y, 0.2f, 0.05f, 0.15f, 0.05f);

	drawCylinder(Position(-LEG_X, LEG_Y, 0), Position(-FOOT_X, 0, 0), 0.05f, 0.03f);
	drawCylinder(Position(LEG_X, LEG_Y, 0), Position(FOOT_X, 0, 0), 0.05f, 0.03f);

	float handX = 0.1f;
	float handY = ARM_Y - 0.4f;
	float z = 0.5f;

	drawCylinder(Position(-ARM_X, ARM_Y, 0), Position(0, handY, z), 0.03f, 0.02f);
	drawCylinder(Position(ARM_X, ARM_Y, 0), Position(handX, handY, z), 0.03f, 0.02f);

	if(!swing) drawCylinder(Position(handX, handY, z), Position(handX, handY + 1.3f, z), 0.02f, 0.04f);
	else drawCylinder(Position(handX, handY, z), Position(handX, 1.0f, z + 0.7f), 0.02f, 0.04f);

	if (swing)
	{
		glPopMatrix();
	}
}
