#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class HullShader
{
private:
	ID3D11HullShader* _hullShader;
	HRESULT createHullShader(LPCWSTR filename, ID3D11Device* device);

public:
	HullShader(LPCWSTR filename, ID3D11Device* device);
	~HullShader();
	ID3D11HullShader & getHullShader();
};