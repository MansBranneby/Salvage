#include "Clock.h"

Clock::Clock()
{
	_deltaSeconds = 0;
	_timeInSec = 0;

	startClock();
}

Clock::~Clock()
{
}

void Clock::startClock()
{
	QueryPerformanceFrequency(&_clockFreq);
	QueryPerformanceCounter(&_startTime);
}

void Clock::reset()
{
	_startTime = _currTime;
}

float Clock::getDeltaSeconds()
{
	QueryPerformanceCounter(&_currTime);
	_delta.QuadPart = _currTime.QuadPart - _startTime.QuadPart;
	_deltaSeconds = (float)_delta.QuadPart / _clockFreq.QuadPart;
	
	return _deltaSeconds;
}

float Clock::getTimeInSec()
{
	return 	_timeInSec += _deltaSeconds;
}
