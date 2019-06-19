#pragma once
#include "Player.h"
#include "State.h"
#include "Clock.h"
#include "InputController.h"
#include <Keyboard.h>
#include <vector>

class State; //Forward declare https://stackoverflow.com/questions/6515143/user-created-header-causing-c2061-syntax-error-identifier-classname
class Game
{
private:
	ID3D11Device* _device; //Hopefully the only place in GameSystem with render stuff.
	ID3D11DeviceContext* _deviceContext;
	InputController* _inputController; //Input from player
	Clock _clock; //Get delta seconds
	std::vector<State*> _states; //Holds states that should be updated
	Player* _player;
public:
	
	Game();
	Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext, InputController* inputController, Clock clock);
	~Game();

	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void handleInput();												   //Calls handleInput of the current state
	void update();											//Calls update on current state
	void draw();													   //Calls draw on current state

	ID3D11DeviceContext* getDeviceContext();
	Player* getPlayer();
	Clock getClock();
	InputController* getInputController();
};

