#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class pixelShader
{
private:
	ID3D11PixelShader* _pixelShader;
	HRESULT createPixelShader(LPCWSTR fileName, ID3D11Device* device);
public:
	pixelShader(LPCWSTR fileName, ID3D11Device* device);

	ID3D11PixelShader & getPixelShader() const;
};