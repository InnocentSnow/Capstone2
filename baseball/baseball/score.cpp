#include <string.h>
#include "score.h"

std::string Score::getResultString()
{
	char buf[128];

	if (m_scoreThisTime)
	{
		_itoa_s(m_scoreThisTime, buf, 10);
		return std::string(m_resultString[m_result]) + "(Score:" + buf + ")";
	}

	return m_resultString[m_result];
}

void Score::init(bool isUserFirst)
{
	m_strike = 0;
	m_ball = 0;
	m_out = 0;

	m_turn = 0;
	m_inning = 1;
	m_currentTeam = isUserFirst ? USER : ENEMY;

	memset(m_totalScore, 0, sizeof(m_totalScore));
	memset(m_score, 0, sizeof(m_score));
	memset(m_isRunnerOn, 0, sizeof(m_isRunnerOn));
	m_scoreThisTime = 0;

	m_resultString[STRIKE] = "STRIKE";
	m_resultString[BALL] = "BALL";
	m_resultString[STRIKE_OUT] = "STRIKE / BATTER OUT";
	m_resultString[FOUR_BALL] = "FOUR BALL";
	m_resultString[THREE_OUT] = "THREE OUT";
	m_resultString[OUTED] = "OUT";
	m_resultString[HIT] = "HIT";
	m_resultString[HOME_RUN] = "HOME RUN";
}

void Score::changeRotation()
{
	m_currentTeam ^= 1;

	m_strike = 0;
	m_ball = 0;
	m_out = 0;

	for (int i = 0; i < 4; ++i) m_isRunnerOn[i] = false;

	++m_turn;
	if (m_turn == 2)
	{
		m_turn = 0;
		++m_inning;
	}
}

void Score::reset()
{
	m_isRunnerOn[3] = false;
	m_scoreThisTime = 0;
}

void Score::makeScore()
{
	++m_score[m_currentTeam][m_inning];
	++m_totalScore[m_currentTeam];
	++m_scoreThisTime;
}

void Score::homeRun()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_isRunnerOn[i])
		{
			++m_score[m_currentTeam][m_inning];
			++m_totalScore[m_currentTeam];
			++m_scoreThisTime;
			m_isRunnerOn[i] = false;
		}
	}

	m_result = HOME_RUN;
}

void Score::strike()
{
	m_result = STRIKE;
	++m_strike;

	if (m_strike == 3)
	{
		out();
		if(m_result != THREE_OUT) m_result = STRIKE_OUT;
	}
}

void Score::ball()
{
	m_result = BALL;
	++m_ball;

	if (m_ball == 4)
	{
		m_strike = 0;
		m_ball = 0;
		m_result = FOUR_BALL;
	}
}

void Score::out()
{
	m_strike = 0;
	m_ball = 0;
	++m_out;

	m_result = OUTED;
	if (m_out == 3)
	{
		m_result = THREE_OUT;
	}
}

void Score::hit()
{
	m_result = HIT;
	m_strike = 0;
	m_ball = 0;
}
