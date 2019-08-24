#include "GameState.h"

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	
	// Check for collision, if there is collision then we don't want to allow the player to move until it's resolved (pushed out of collision)
	 //Not safe, will crasch if you try to access gameobject outside of array.
	bool colliding = false;
	Player* player = game->getLevelHandler()->getPlayer();
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();

	for (int i = 0; i < nrOfObjects && colliding == false; ++i)
		colliding = player->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume());

	// Handle input if there is no collison
	if (colliding == false)
	{
		player->handleInput(kb, gp, deltaSeconds);
	}
	
}

void GameState::update(Game* game)
{
	game->updateCameraBuffers();

	// currently just moves player in the opposite direction until no collision. This also prevents player from "gliding along the surface" which isn't good.
	// In order to fix not being able to slide along the surface one has to find the collision normal and push out that way
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	Player* player = game->getLevelHandler()->getPlayer();
	bool colliding = false;

	for (int i = 0; i < nrOfObjects && colliding == false; i++)
		colliding = player->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()); //Not safe, will crasch if you try to access gameobject outside of array.		

	// if collision between player and object change travel direction
	if (colliding)
	{
		player->move(-player->getAcceleration(), game->getClock()->getDeltaSeconds());
	}
	else
		player->move(player->getAcceleration(), game->getClock()->getDeltaSeconds());
	
	// Set player height to terrain
	Terrain* terrain = game->getLevelHandler()->getTerrain(0); // currently only works for one terrain
	float height = terrain->getHeight(player->getPositionFloat3().x, player->getPositionFloat3().z);
	player->setHeight(height);

	game->getCamera()->followObject(player->getPositionVector(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	// Draw active level. 
	// A level may contain terrain, objects and a player
	game->getLevelHandler()->drawLevel(game->getGraphicResources(), game->getClock()->getTimeInSec());      
}
