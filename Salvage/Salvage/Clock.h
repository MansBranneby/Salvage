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

public:
	Clock();
	~Clock();

	float getDeltaSeconds();
	void refreshClock(); // sets _currTime to to _startTime otherwise deltaSeconds grows too big.
};