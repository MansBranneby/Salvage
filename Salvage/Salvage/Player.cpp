#include "Player.h"

Player::Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:Robot(device, deviceContext, PLAYER, startingPosition, modelFile)
{

}

Player::~Player()
{
}

void Player::handleInput(Keyboard::State kb, GamePad::State gp, float deltaSeconds)
{
	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	float deltaSpeed = 0.65f * deltaSeconds; // Player speed * delta seconds = delta speed
	float diagonalDeltaSpeed = deltaSpeed * cos(DirectX::XM_PI / 4); // Delta speed * cos(pi/4) = diogonal delta speed (if you don't multiply with cos(pi/4) the player will move double as fast diagonally)
	XMVECTOR acceleration{ 0.0f, 0.0f, 0.0f, 0.0f };

	//Keyboard
	if (kb.W) //Forward
		acceleration = { 0.0f, 0.0f, deltaSpeed, 0.0f };
	if (kb.S) //Backwards
		acceleration = { 0.0f, 0.0f, -deltaSpeed, 0.0f };
	if (kb.D)	//Right
		acceleration = { deltaSpeed, 0.0f, 0.0f, 0.0f };
	if (kb.A)	//Left
		acceleration = { -deltaSpeed, 0.0f, 0.0f, 0.0f };
	if (kb.W && kb.D) // Diagonal forward right
		acceleration = { diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
	if (kb.S && kb.D) // Diagonal backward right
		acceleration = { diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };
	if (kb.W && kb.A) // Diagonal forward left
		acceleration = { -diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
	if (kb.S && kb.A) // Diagonal backward left
		acceleration = { -diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };


	//Controller
	if (gp.IsLeftThumbStickUp()) //Forward
		acceleration = { 0.0f, 0.0f, deltaSpeed, 0.0f };
	if (gp.IsLeftThumbStickDown()) //Backwards
		acceleration = { 0.0f, 0.0f, -deltaSpeed, 0.0f };
	if (gp.IsLeftThumbStickRight())	//Right
		acceleration = { deltaSpeed, 0.0f, 0.0f, 0.0f };
	if (gp.IsLeftThumbStickLeft())	//Left
		acceleration = { -deltaSpeed, 0.0f, 0.0f, 0.0f };
	if (gp.IsLeftThumbStickUp() && gp.IsLeftThumbStickRight()) // Diagonal forward right
		acceleration = { diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
	if (gp.IsLeftThumbStickDown() && gp.IsLeftThumbStickRight()) // Diagonal backward right
		acceleration = { diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };
	if (gp.IsLeftThumbStickUp() && gp.IsLeftThumbStickLeft()) // Diagonal forward left
		acceleration = { -diagonalDeltaSpeed, 0.0f, diagonalDeltaSpeed, 0.0f };
	if (gp.IsLeftThumbStickDown() && gp.IsLeftThumbStickLeft()) // Diagonal backward left
		acceleration = { -diagonalDeltaSpeed, 0.0f, -diagonalDeltaSpeed, 0.0f };

	setAcceleration(acceleration);
}


