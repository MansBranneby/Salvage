#include "TerrainVertex.h"

TerrainVertex::TerrainVertex()
{
	_position = { 0.0f, 0.0f, 0.0f };
	_textureCoords = { 0.0f, 0.0f };
	_boundsY = { 0.0f, 0.0f };
}

TerrainVertex::~TerrainVertex()
{
}
