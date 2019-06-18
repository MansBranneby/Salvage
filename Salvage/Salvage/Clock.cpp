#include "Clock.h"

Clock::Clock()
{
	QueryPerformanceFrequency(&_clockFreq);
	QueryPerformanceCounter(&_startTime);
}

Clock::~Clock()
{
}

void Clock::startClock()
{
	QueryPerformanceFrequency(&_clockFreq);
	QueryPerformanceCounter(&_startTime);
}

float Clock::getDeltaSeconds()
{
	QueryPerformanceCounter(&_currTime);
	_delta.QuadPart = _currTime.QuadPart - _startTime.QuadPart;
	float deltaSeconds = (float)_delta.QuadPart / (_clockFreq.QuadPart * 1000.0f);
	_startTime = _currTime;
	
	return deltaSeconds;
}

void Clock::startAnimation()
{
	QueryPerformanceCounter(&_animationStart);
}

float Clock::getAnimationTime()
{
	QueryPerformanceCounter(&_currTime);
	_delta.QuadPart = _currTime.QuadPart - _animationStart.QuadPart;
	float deltaSeconds = (float)_delta.QuadPart / _clockFreq.QuadPart;

	return deltaSeconds;
}
