#include "GameState.h"

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.

	//Keyboard
	if (kb.W) //Forward
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, deltaSeconds * 25.0f)); //Update with player speed
	if (kb.S) //Backwards
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, -deltaSeconds * 25.0f));
	if (kb.D)	//Right
		game->getPlayer()->move(DirectX::XMFLOAT3(deltaSeconds * 25.0f, 0.0f, 0.0f));
	if (kb.A)	//Left
		game->getPlayer()->move(DirectX::XMFLOAT3(-deltaSeconds * 25.0f, 0.0f, 0.0f));
	
	//Controller
	if(gp.IsLeftThumbStickUp())
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, deltaSeconds * 25.0f));
	if (gp.IsLeftThumbStickDown()) //Backwards
		game->getPlayer()->move(DirectX::XMFLOAT3(0.0f, 0.0f, -deltaSeconds * 25.0f));
	if (gp.IsLeftThumbStickRight())	//Right
		game->getPlayer()->move(DirectX::XMFLOAT3(deltaSeconds * 25.0f, 0.0f, 0.0f));
	if (gp.IsLeftThumbStickLeft())	//Left
		game->getPlayer()->move(DirectX::XMFLOAT3(-deltaSeconds * 25.0f, 0.0f, 0.0f));
}

void GameState::update(Game* game)
{
	
	if (game->getPlayer()->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(0)->getBoundingVolume())) //Not safe, will crasch if you try to access gameobject outside of array.
		game->getPlayer()->move(DirectX::XMFLOAT3(-game->getClock()->getDeltaSeconds() * 25.0f, 0.0f, 0.0f ));

	game->getPlayer()->updateLogic();                                  //Update player
	game->getCamera()->followObject(game->getPlayer()->getPosition(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	game->getPlayer()->draw(game->getDeviceContext()); //Draw player
	game->getLevelHandler()->drawLevel();              //Draw level

	//game->getPlayer()->drawBoundingVolume(game->getDeviceContext());
}
