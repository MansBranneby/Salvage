#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class PixelShader
{
private:
	ID3D11PixelShader* _pixelShader;
	HRESULT createPixelShader(LPCWSTR filename, ID3D11Device* device);
public:
	PixelShader();
	PixelShader(LPCWSTR filename, ID3D11Device* device);
	~PixelShader();

	ID3D11PixelShader & getPixelShader() const;
};