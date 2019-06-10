#include "Clock.h"

Clock::Clock()
{
	QueryPerformanceFrequency(&_clockFreq);
	QueryPerformanceCounter(&_startTime);
}

Clock::~Clock()
{
}

float Clock::getDeltaSeconds()
{
	QueryPerformanceCounter(&_currTime);
	_delta.QuadPart = _currTime.QuadPart - _startTime.QuadPart;
	float deltaSeconds = (float)_delta.QuadPart / _clockFreq.QuadPart;
	_startTime = _currTime;

	return deltaSeconds;
}

void Clock::refreshClock()
{
	QueryPerformanceCounter(&_currTime);
	_startTime = _currTime;
}
