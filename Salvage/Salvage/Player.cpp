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
