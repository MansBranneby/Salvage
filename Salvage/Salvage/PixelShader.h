#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class PixelShader
{
private:
	ID3D11PixelShader* _pixelShader;
	HRESULT createPixelShader(LPCWSTR fileName, ID3D11Device* device);
public:
	PixelShader();
	PixelShader(LPCWSTR fileName, ID3D11Device* device);

	ID3D11PixelShader & getPixelShader() const;
};