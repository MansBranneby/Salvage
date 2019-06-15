#include "Game.h"

Game::Game(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;
	
	//Spawn location of player
	DirectX::XMVECTOR startingPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); //Player och denna ska nog skapas och sättas i LevelAi för att sedan skicka en referens till Game. 
	_player = new Player(_device, _deviceContext, startingPosition);
}

Game::~Game()
{
	delete _player;
}

void Game::update(DirectX::Keyboard::State kb, float deltaSeconds)
{

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_player->move(DirectX::XMFLOAT3(0.0f, 0.0f, deltaSeconds*100.0f)); //Update with player speed
	if (kb.S) //Backwards
		_player->move(DirectX::XMFLOAT3(0.0f, 0.0f, -deltaSeconds * 100.0f));
	if (kb.A)	//Left
		_player->move(DirectX::XMFLOAT3(-deltaSeconds * 100.0f, 0.0f, 0.0f));
	if (kb.D)	//Right
		_player->move(DirectX::XMFLOAT3(deltaSeconds * 100.0f, 0.0f, 0.0f));

	_player->updateLogic();
}

void Game::draw()
{
	//Senare gå igenom alla GameObject
	//_player->_model.draw(_deviceContext);
	_player->draw(_deviceContext);
}