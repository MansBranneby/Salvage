#pragma once

#include <windows.h>


class Clock
{
private:
	// TID
	LARGE_INTEGER _clockFreq;
	LARGE_INTEGER _startTime;
	LARGE_INTEGER _delta;
	LARGE_INTEGER _currTime;
	LARGE_INTEGER _animationStart;

public:
	Clock();
	~Clock();

	void startClock();
	float getDeltaSeconds();
	void startAnimation();
	float getAnimationTime();
};