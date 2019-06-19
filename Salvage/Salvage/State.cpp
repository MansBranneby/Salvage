#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::pause()
{
	_pause = true;
}

void State::resume()
{
	_pause = false;
}

bool State::isPaused()
{
	return _pause;
}
