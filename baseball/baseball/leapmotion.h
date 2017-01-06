#ifndef _LEAP_MOTION_H
#define _LEAP_MOTION_H

#include <Windows.h>
#include <deque>
#include "Leap.h"
#include "input.h"

class LeapMotion;

struct LeapData
{
	DWORD time;
	float value;
	float fps;

	LeapData(DWORD _time, float _value, float _fps) : time(_time), value(_value), fps(_fps) {}
};

class LeapListener : public Leap::Listener
{

public:

	LeapListener(LeapMotion *leapMotion);

	void onConnect(const Leap::Controller &controller);
	void onDisconnect(const Leap::Controller &controller);
	void onFrame(const Leap::Controller &controller);

private:

	LeapMotion *m_leapMotion;
};

class LeapMotion : public Input
{

public:

	virtual float getX();

	void connected() { m_isConnected = true; }
	void disconnected() { m_isConnected = false; }
	void input(float value, float fps);

private:

	LeapMotion();
	virtual ~LeapMotion();
	
	void popOldData(DWORD time);

	Leap::Controller m_controller;
	LeapListener m_listener;

	float m_sum;
	bool m_isConnected;
	std::deque<LeapData> m_deque;

	friend class Input;
};


#endif
