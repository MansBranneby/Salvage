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
	_states.push_back(state);
}

void Game::handleInput()
{
	_states.back()->handleInput(this);
}

void Game::update()
{
	_states.back()->update(this);
	//_player->updateLogic();
}

void Game::draw()
{
	//Senare gå igenom alla GameObject
	//_player->_model.draw(_deviceContext);
	_player->draw(_deviceContext);
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
