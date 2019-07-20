#pragma once
#include "State.h"

class GameState : public State
{
public:
	virtual void handleInput(Game* game);
	virtual void update(Game* game);
	virtual void draw(Game* game, GraphicResources * graphicResources);
};