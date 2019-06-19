#include "GameState.h"

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	float deltaSeconds = game->getClock().getDeltaSeconds();

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, deltaSeconds)); //Update with player speed
	if (kb.S) //Backwards
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, -deltaSeconds));
	if (kb.A)	//Left
		game->getPlayer()->move(DirectX::XMFLOAT3(-deltaSeconds, 0.0f, 0.0f));
	if (kb.D)	//Right
		game->getPlayer()->move(DirectX::XMFLOAT3(deltaSeconds, 0.0f, 0.0f));
}

void GameState::update(Game* game)
{
	game->getPlayer()->updateLogic();                                  //Update player
	game->getCamera()->followObject(game->getPlayer()->getPosition()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	game->getPlayer()->draw(game->getDeviceContext()); //Draw player
	game->getLevelHandler()->drawLevel();              //Draw level
}
