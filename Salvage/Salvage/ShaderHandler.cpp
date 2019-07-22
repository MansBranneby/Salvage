#include "ShaderHandler.h"

ShaderHandler::ShaderHandler()
{
}

ShaderHandler::ShaderHandler(ID3D11Device* device)
{
	// Create vertex shaders
	_objectVS = new VertexShader(L"VertexShader.hlsl", device, POSITION_NORMAL_TEXTURE);
	_boundingVolumeVS = new VertexShader(L"BoundingVolumeVS.hlsl", device, POSITION_COLOUR);
	_terrainVS = new VertexShader(L"TerrainVS.hlsl", device, POSITION_NORMAL_TEXTURE);

	// Create pixel shaders
	_objectPS = new PixelShader(L"PixelShader.hlsl", device);
	_boundingVolumePS = new PixelShader(L"BoundingVolumePS.hlsl", device);
	_terrainPS = new PixelShader(L"TerrainPS.hlsl", device);
}

ShaderHandler::~ShaderHandler()
{
	// Delete vertex shaders
	if (_terrainVS)
		delete _terrainVS;
	if (_objectVS)
		delete _objectVS;
	if (_boundingVolumeVS)
		delete _boundingVolumeVS;

	// Delete pixel shaders
	if (_terrainPS)
		delete _terrainPS;
	if (_objectPS)
		delete _objectPS;
	if (_boundingVolumePS)
		delete _boundingVolumePS;

}

VertexShader * ShaderHandler::getTerrainVS()
{
	return _terrainVS;
}

VertexShader * ShaderHandler::getObjectVS()
{
	return _objectVS;
}

VertexShader * ShaderHandler::getBoundingVolumeVS()
{
	return _boundingVolumeVS;
}

PixelShader * ShaderHandler::getTerrainPS()
{
	return _terrainPS;
}

PixelShader * ShaderHandler::getObjectPS()
{
	return _objectPS;
}

PixelShader * ShaderHandler::getBoundingVolumePS()
{
	return _boundingVolumePS;
}
