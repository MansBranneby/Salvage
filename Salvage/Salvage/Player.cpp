#include "Player.h"

Player::Player(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMVECTOR startingPosition)
	:Robot(device, deviceContext, PLAYER, startingPosition)
{
}

Player::~Player()
{
}

void Player::abstract()
{
}
