#pragma once
#include "GameObjectHandler.h"

class LevelAI
{
private:
public:
	LevelAI();
	~LevelAI();

	void createLevel(int levelNr, GameObjectHandler* objHandler);
};