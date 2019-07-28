#pragma once
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "PixelShader.h"

class ShaderHandler
{
private:
	// Vertex shaders (VS)
	VertexShader* _terrainVS;
	VertexShader* _objectVS;
	VertexShader* _boundingVolumeVS;
	// Hull shaders (HS)
	HullShader* _terrainHS;
	// Domain shaders (DS)
	DomainShader* _terrainDS;
	// Pixel shaders (PS)
	PixelShader* _terrainPS;
	PixelShader* _objectPS;
	PixelShader* _boundingVolumePS;

public:
	ShaderHandler();
	ShaderHandler(ID3D11Device* device);
	~ShaderHandler();
	// VS
	VertexShader* getTerrainVS();
	VertexShader* getObjectVS();
	VertexShader* getBoundingVolumeVS();
	// HS
	HullShader* getTerrainHS();
	// DS
	DomainShader* getTerrainDS();
	// PS
	PixelShader* getTerrainPS();
	PixelShader* getObjectPS();
	PixelShader* getBoundingVolumePS();

};
