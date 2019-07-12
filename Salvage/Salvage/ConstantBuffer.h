#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ConstantBuffer
{
private:
	ID3D11Buffer* _constantBuffer = nullptr;
	void createConstantBuffer(ID3D11Device* device, const void* data, UINT dataSize);

public:
	ConstantBuffer(ID3D11Device* device, const void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer** getConstantBuffer();
};