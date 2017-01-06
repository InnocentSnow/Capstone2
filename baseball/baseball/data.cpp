
#include <Windows.h>
#include "data.h"
#include "util.h"
#include "game.h"

void Data::setDefencerTarget(DWORD currentTime, const Position &target)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	for (size_t i = 0; i < data->defencer.size(); ++i)
	{
		Position defencer = data->defencer[i]->getPosition(currentTime);
		data->defencer[i]->setData(defencer, target, 0, currentTime, currentTime + (DWORD)(calcHorizontalDist(defencer, target) / PLAYER_SPEED));
	}
}

void Data::stopDefencer(DWORD currentTime)
{
	Game *game = Game::getInstance();
	Data *data = game->getData();

	for (size_t i = 0; i < data->defencer.size(); ++i)
	{
		Position defencer = data->defencer[i]->getPosition(currentTime);
		data->defencer[i]->setData(defencer, defencer, 0, currentTime, currentTime);
	}
}
Position BallData::calcPosition(DWORD currentTime)
{
	if (currentTime < m_startTime) currentTime = m_startTime;
	if (m_endTime < currentTime) currentTime = m_endTime;
	float portion = getPortion(m_startTime, m_endTime, currentTime);

	float x = linearInterpolate(m_start.x, m_end.x, portion);
	float z = linearInterpolate(m_start.z, m_end.z, portion);
	float y;

	DWORD halfTime = (m_startTime + m_endTime) / 2;
	if (currentTime < halfTime) y = squareInterpolate(m_start.y, m_maxY, getPortion(m_startTime, halfTime, currentTime));
	else y = squareInterpolate(m_maxY, m_end.y, getPortion(halfTime, m_endTime, currentTime));

	return Position(x, y, z);
}

Position BallData::getPosition(DWORD currentTime)
{
	if (currentTime < m_startTime) currentTime = m_startTime;
	if (m_endTime < currentTime) currentTime = m_endTime;
	Position p = calcPosition(currentTime);

	if (STADIUM_RADIUS * STADIUM_RADIUS <= p.x * p.x + p.z * p.z)
	{
		Game *game = Game::getInstance();
		Data *data = game->getData();

		Position last = calcPosition(m_lastTime);
		if (last.x * last.x + last.z * last.z < STADIUM_RADIUS * STADIUM_RADIUS && p.y <= FENCE_HEIGHT)
		{
			DWORD targetTime = m_lastTime - (data->ball.m_endTime - m_lastTime) / 4;
			Position targetPosition = getPosition(targetTime);
			targetPosition.y = 0;

			setBallData(last, targetPosition, last.y / 2, currentTime, m_endTime);
			data->setDefencerTarget(currentTime, m_end);

			return last;
		}
		else
		{
			data->stopDefencer(currentTime);
		}
	}

	m_lastTime = currentTime;
	return p;
}

void BallData::setBallData(const Position &start, const Position &end, float maxY, DWORD startTime, DWORD endTime)
{
	m_start = start;
	m_end = end;
	m_maxY = maxY;

	m_startTime = startTime;
	m_endTime = endTime;
}
