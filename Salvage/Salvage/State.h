#pragma once
#include "Game.h"

//Possible way of handling game states: http://gamedevgeek.com/tutorials/managing-game-states-in-c/

class Game; //Forward declare https://stackoverflow.com/questions/6515143/user-created-header-causing-c2061-syntax-error-identifier-classname
class State
{
private:
	bool _pause;

public:
	State();
	~State();
	//Example of usage: pause game and bring up menu state, then resume after exiting menu state
	virtual void pause(); 
	virtual void resume();

	virtual void handleInput(Game* game) = 0; //Handle input
	virtual void update(Game* game) = 0;	  //Example: update player position
	virtual void draw(Game* game) = 0;	      //Draw updated models
};

