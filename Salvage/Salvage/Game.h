#pragma once
#include "LevelHandler.h"
#include "Player.h"
#include "State.h"
#include "Clock.h"
#include "InputController.h"
#include "Camera.h"
#include <Keyboard.h>
#include <vector>

class State; //Forward declare https://stackoverflow.com/questions/6515143/user-created-header-causing-c2061-syntax-error-identifier-classname
class Game
{
private:
	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;
	Camera* _camera = nullptr;
	Player* _player = nullptr;
	InputController* _inputController = nullptr; //Input from player
	Clock* _clock = nullptr; //Get delta seconds
	std::vector<State*> _states; //Holds states that should be updated
	//GameObject* _staticObject; //test

	LevelHandler* _levelHandler = nullptr;

	void updateCamera();
	void updateCameraBuffers();
	


public:
	Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float height, Clock* clock, InputController* inputController);
	~Game();

	void changeState(State* state);
	void pushState(State* state);
	void popState();

	void handleInput();												   //Calls handleInput of the current state
	void update();											//Calls update on current state
	void draw();													   //Calls draw on current state

	ID3D11DeviceContext* getDeviceContext();
	Player* getPlayer();
	Clock* getClock();
	InputController* getInputController();
	LevelHandler* getLevelHandler();
	Camera* getCamera();
};

