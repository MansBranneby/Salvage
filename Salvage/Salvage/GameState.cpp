#include "GameState.h"

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	

	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	bool colliding = false;
	for (int i = 0; i < nrOfObjects && colliding == false; i++)
		colliding = game->getPlayer()->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()); //Not safe, will crasch if you try to access gameobject outside of array.

	if (colliding == false)
	{
		game->getPlayer()->handleInput(kb, gp, deltaSeconds);

	
	}
	
}

void GameState::update(Game* game)
{
	// currently just moves player in the opposite direction until no collision. This also prevents player from "gliding along the surface" which isn't good.
	// In order to fix not being able to slide along the surface one has to find the collision normal and push out that way
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	bool colliding = false;
	for (int i = 0; i < nrOfObjects && colliding == false; i++)
		colliding = game->getPlayer()->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()); //Not safe, will crasch if you try to access gameobject outside of array.		
	// if collision between player and object change travel direction
	if (colliding)
		game->getPlayer()->move(-game->getPlayer()->getVelocity());
	else
		game->getPlayer()->move(game->getPlayer()->getVelocity());
	
	// Set player height to terrain
	Terrain* terrain = game->getLevelHandler()->getTerrain(0);
	float height = terrain->getHeight(DirectX::XMVectorGetX(game->getPlayer()->getPosition()), DirectX::XMVectorGetZ(game->getPlayer()->getPosition()));
	game->getPlayer()->setHeight(height);

	game->getPlayer()->updateLogic();																	    //Update player
	game->getCamera()->followObject(game->getPlayer()->getPosition(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	game->getPlayer()->draw(game->getDeviceContext()); //Draw player
	game->getLevelHandler()->drawLevel();              //Draw level
}
