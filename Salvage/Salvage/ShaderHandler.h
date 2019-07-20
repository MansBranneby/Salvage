#pragma once
#include "VertexShader.h"
#include "pixelShader.h"

class ShaderHandler
{
private:
	// Vertex shaders (VS)
	VertexShader* _terrainVS;
	VertexShader* _objectVS;
	VertexShader* _boundingVolumeVS;
	// Pixel shaders (PS)
	PixelShader* _terrainPS;
	PixelShader* _objectPS;
	PixelShader* _boundingVolumePS;
public:
	ShaderHandler();
	ShaderHandler(ID3D11Device* device);
	~ShaderHandler();
	
	VertexShader* getTerrainVS();
	VertexShader* getObjectVS();
	VertexShader* getBoundingVolumeVS();

	PixelShader* getTerrainPS();
	PixelShader* getObjectPS();
	PixelShader* getBoundingVolumePS();
};
