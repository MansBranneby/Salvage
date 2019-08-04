#pragma once
#include "State.h"

class GameState : public State
{
public:
	void handleInput(Game* game);
	void update(Game* game);
	void draw(Game* game);
};