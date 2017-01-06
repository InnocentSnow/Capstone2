#include <windows.h>
#include <stdio.h>
#include "leapmotion.h"
#include "util.h"

using namespace Leap;
const float LEAP_LIMIT = 120000;
const DWORD AVG_TIME = 100;

LeapListener::LeapListener(LeapMotion *leapMotion) : m_leapMotion(leapMotion)
{

}

void LeapListener::onConnect(const Leap::Controller &controller)
{
	m_leapMotion->connected();
	printf("connected\n");
}

void LeapListener::onDisconnect(const Leap::Controller &controller)
{
	m_leapMotion->disconnected();
	printf("disconnected\n");
}

void LeapListener::onFrame(const Leap::Controller &controller)
{
	Frame frame = controller.frame();

	 if (frame.hands().count())
	 {
		const Hand &hand = *frame.hands().begin();
		float val = hand.palmVelocity().magnitude();

		if (LEAP_LIMIT < val * frame.currentFramesPerSecond())
		{
			m_leapMotion->input(val, frame.currentFramesPerSecond());
		}
	 }
}

LeapMotion::LeapMotion() : m_listener(this), m_isConnected(false), m_sum(0)
{
	m_controller.addListener(m_listener);
}

LeapMotion::~LeapMotion()
{
	m_controller.removeListener(m_listener);
}

void LeapMotion::popOldData(DWORD time)
{
	while (!m_deque.empty() && m_deque.front().time < time - AVG_TIME)
	{
		LeapData data = m_deque.front();
		m_deque.pop_front();

		m_sum -= data.value * data.fps;
	}
}

float LeapMotion::getX()
{
	DWORD time = getTime();
	popOldData(time);

	if (m_deque.empty()) return 0;

	float avg = m_sum / m_deque.size();
	return avg / LEAP_LIMIT;
}

void LeapMotion::input(float value, float fps)
{
	DWORD time = getTime();
	popOldData(time);

	m_sum += value * fps;
	m_deque.push_back(LeapData(time, value, fps));
}