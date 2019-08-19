#pragma once

#include <windows.h>


class Clock
{
private:
	LARGE_INTEGER _clockFreq;
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _currTime;
	LARGE_INTEGER _delta;
	float _deltaSeconds;
	float _timeInSec;

public:
	Clock();
	~Clock();

	void startClock();
	void reset(); 
	float getDeltaSeconds();
	// time since the start of the program
	float getTimeInSec();
};