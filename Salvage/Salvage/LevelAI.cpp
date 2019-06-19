#include "LevelAI.h"

LevelAI::LevelAI()
{
}

LevelAI::~LevelAI()
{
}

void LevelAI::createLevel(int levelNr, GameObjectHandler * objHandler)
{
	switch (levelNr)
	{
	case 1:
		objHandler->addGameObject(XMVectorSet(5.0f, 0.0f, 0.0f, 1.0f), STATICOBJECT);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}
}
