#include "Game.h"

Game::Game()
{
	//Spawn location of player
	DirectX::XMVECTOR startingPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f); //Player och denna ska nog skapas och sättas i LevelAi för att sedan skicka en referens till Game. 
	//_player = new GameObject(startingPosition);
}

Game::~Game()
{

}
