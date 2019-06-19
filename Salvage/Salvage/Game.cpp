#include "Game.h"

Game::Game()
{
}

Game::Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext, InputController* inputController, Clock clock)
{
	_device = device;
	_deviceContext = deviceContext;
	_inputController = inputController;
	_clock = clock;
	//Spawn location of player
	DirectX::XMVECTOR startingPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //Player och denna ska nog skapas och sättas i LevelAi för att sedan skicka en referens till Game. 
	_player = new Player(_device, _deviceContext, startingPosition);
}

Game::~Game()
{
	delete _player;
}

void Game::changeState(State * state)
{
	//Check if state stack has at least one state
	//Pause the current state
	//Add the new state
	//Example: pause game state, add menu state
	if (!_states.empty())
	{
		_states.back()->pause();
		_states.push_back(state);
	}
}

void Game::pushState(State * state)
{
	//Add a state ontop of current
	//Example: maybe you want to update game while you're in a menu
	_states.push_back(state);
}

void Game::popState()
{
	//Delete the current state
	//resume the paused state
	if (!_states.empty())
	{
		_states.pop_back();

		if(!_states.empty())
			_states.back()->resume();
	}
}

void Game::handleInput()
{
	//Handle input if not paused
	for(int i = 0; i < _states.size(); i++)
		if(!_states[i]->isPaused())
			_states[i]->handleInput(this);
}

void Game::update()
{
	//Update game logic if not paused
	for (int i = 0; i < _states.size(); i++)
		if (!_states[i]->isPaused())
			_states[i]->update(this);
}

void Game::draw()
{
	//Draw models if not paused
	for (int i = 0; i < _states.size(); i++)
		if (!_states[i]->isPaused())
			_states[i]->draw(this);
}

ID3D11DeviceContext * Game::getDeviceContext()
{
	return _deviceContext;
}

Player* Game::getPlayer()
{
	return _player;
}

Clock Game::getClock()
{
	return _clock;
}

InputController* Game::getInputController()
{
	return _inputController;
}
