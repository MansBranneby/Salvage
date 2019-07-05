#include "GameState.h"

void GameState::handleInput(Game* game)
{
	DirectX::Keyboard::State kb = game->getInputController()->getKeyboardState();
	DirectX::GamePad::State gp = game->getInputController()->getGamePadState();
	float deltaSeconds = game->getClock()->getDeltaSeconds();

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	float deltaSpeed = game->getPlayer()->getSpeed() * deltaSeconds; // Player speed * delta seconds = delta speed
	float diagonalDeltaSpeed = deltaSpeed * cos(DirectX::XM_PI / 4); // Delta speed * cos(pi/4) = diogonal delta speed (if you don't multiply with cos(pi/4) the player will move double as fast diagonally)
	XMVECTOR playerVelocity{ 0.0f, 0.0f, 0.0f, 0.0f };

	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	bool colliding = false;
	for (int i = 0; i < nrOfObjects && colliding == false; i++)
		colliding = game->getPlayer()->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()); //Not safe, will crasch if you try to access gameobject outside of array.

	if (colliding == false)
	{
		//Keyboard
		if (kb.W) //Forward
			playerVelocity = { 0.0f, 0.0f, deltaSpeed, 0.0f };
		if (kb.S) //Backwards
			playerVelocity = { 0.0f, 0.0f, -deltaSpeed, 0.0f };
		if (kb.D)	//Right
			playerVelocity = { deltaSpeed, 0.0f, 0.0f, 0.0f };
		if (kb.A)	//Left
			playerVelocity = { -deltaSpeed, 0.0f, 0.0f, 0.0f };
		if (kb.W && kb.D) // Diagonal forward right
			playerVelocity = { diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
		if (kb.S && kb.D) // Diagonal backward right
			playerVelocity = { diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };
		if (kb.W && kb.A) // Diagonal forward left
			playerVelocity = { -diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
		if (kb.S && kb.A) // Diagonal backward left
			playerVelocity = { -diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };

		//Controller
		if (gp.IsLeftThumbStickUp()) //Forward
			playerVelocity = { 0.0f, 0.0f, deltaSpeed, 0.0f };
		if (gp.IsLeftThumbStickDown()) //Backwards
			playerVelocity = { 0.0f, 0.0f, -deltaSpeed, 0.0f };
		if (gp.IsLeftThumbStickRight())	//Right
			playerVelocity = { deltaSpeed, 0.0f, 0.0f, 0.0f };
		if (gp.IsLeftThumbStickLeft())	//Left
			playerVelocity = { -deltaSpeed, 0.0f, 0.0f, 0.0f };
		if (gp.IsLeftThumbStickUp() && gp.IsLeftThumbStickRight()) // Diagonal forward right
			playerVelocity = { diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
		if (gp.IsLeftThumbStickDown() && gp.IsLeftThumbStickRight()) // Diagonal backward right
			playerVelocity = { diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };
		if (gp.IsLeftThumbStickUp() && gp.IsLeftThumbStickLeft()) // Diagonal forward left
			playerVelocity = { -diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
		if (gp.IsLeftThumbStickDown() && gp.IsLeftThumbStickLeft()) // Diagonal backward left
			playerVelocity = { -diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };
	
		game->getPlayer()->setVelocity(playerVelocity);
	}
	
}

void GameState::update(Game* game)
{
	// if collision between player and other gameobject do something
	// currently just moves player in the opposite direction until no collision. This also prevents player from "gliding along the surface" which isn't good.
	// In order to fix not being able to slide along the surface one has to find the collision normal and push out that way
	int nrOfObjects = game->getLevelHandler()->getNrOfGameObjects();
	bool colliding = false;

	for (int i = 0; i < nrOfObjects && colliding == false; i++)
		colliding = game->getPlayer()->getBoundingVolume()->intersectsWithOBB(game->getLevelHandler()->getGameObject(i)->getBoundingVolume()); //Not safe, will crasch if you try to access gameobject outside of array.		
	// move player in the opposite direction if collision is detected
	if (colliding)
		game->getPlayer()->move(-game->getPlayer()->getVelocity());
	else
		game->getPlayer()->move(game->getPlayer()->getVelocity());


	game->getPlayer()->updateLogic();																	    //Update player
	game->getCamera()->followObject(game->getPlayer()->getPosition(), game->getClock()->getDeltaSeconds()); //Update camera based on player position
}

void GameState::draw(Game* game)
{
	game->getPlayer()->draw(game->getDeviceContext()); //Draw player
	game->getLevelHandler()->drawLevel();              //Draw level
}
