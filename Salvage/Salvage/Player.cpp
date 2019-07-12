#include "Player.h"

Player::Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition, std::string modelFile)
	:Robot(device, deviceContext, PLAYER, startingPosition, modelFile)
{

}

Player::~Player()
{
}

void Player::abstract()
{
}

void Player::setHeight(float height)
{
	_position = DirectX::XMVectorSet(DirectX::XMVectorGetX(_position), height + 1.5f, DirectX::XMVectorGetZ(_position), 1.0f);
}

void Player::handleInput(Keyboard::State kb, GamePad::State gp, float deltaSeconds)
{
	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	float deltaSpeed = getSpeed() * deltaSeconds; // Player speed * delta seconds = delta speed
	float diagonalDeltaSpeed = deltaSpeed * cos(DirectX::XM_PI / 4); // Delta speed * cos(pi/4) = diogonal delta speed (if you don't multiply with cos(pi/4) the player will move double as fast diagonally)
	XMVECTOR playerVelocity{ 0.0f, 0.0f, 0.0f, 0.0f };

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

	setVelocity(playerVelocity);
}


