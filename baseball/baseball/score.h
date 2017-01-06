#ifndef _SCORE_H
#define _SCORE_H

#include <map>
#include <string>

class Score
{

public:

	enum UserType
	{
		USER = 0,
		ENEMY = 1
	};

	enum Result
	{
		STRIKE,
		BALL,
		STRIKE_OUT,
		FOUR_BALL,
		THREE_OUT,
		OUTED,
		HIT,
		HOME_RUN
	};

private:

	std::map<Result, const char *> m_resultString;
	Result m_result;

	int m_ball;
	int m_strike;
	int m_out;

	int m_inning;
	int m_turn;
	int m_currentTeam;

	int m_score[2][1000];
	int m_totalScore[2];
	int m_scoreThisTime;

	bool m_isUserFirst;
	bool m_isRunnerOn[4];

public:

	Score() : m_isUserFirst(true) {};

	void init(bool isUserFirst);
	void changeRotation();

	void strike();
	void ball();
	void out();
	void hit();
	void makeScore();
	void homeRun();

	void reset();
	void setRunnerOn(int number, bool value) { m_isRunnerOn[number - 1] = value; }

	bool isUserFirst() { return m_isUserFirst; }
	bool isRunnerOn(int number) { return m_isRunnerOn[number - 1]; }

	int getBall() { return m_ball; }
	int getStrike() { return m_strike; }
	int getOut() { return m_out; }

	int getScore(UserType type, int inning) { return m_score[type][inning]; }
	int getTotalScore(UserType type) { return m_totalScore[type]; }
	
	int getInning() { return m_inning; }
	int getTurn() { return m_turn; }

	Result getResult() { return m_result; }
	std::string getResultString();
};

#endif
