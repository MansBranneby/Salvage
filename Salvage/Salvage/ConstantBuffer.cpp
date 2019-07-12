#include "ConstantBuffer.h"


void ConstantBuffer::createConstantBuffer(ID3D11Device* device, const void* data, UINT dataSize)
{
	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = dataSize;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = data;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// create a Constant Buffer
	HRESULT result = device->CreateBuffer(&cbDesc, &InitData, &_constantBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"Error CreateConstantBuffer", L"Error", MB_OK | MB_ICONERROR);
}

ConstantBuffer::ConstantBuffer(ID3D11Device* device, const void* data, UINT dataSize)
{
	createConstantBuffer(device, data, dataSize);
}

ConstantBuffer::~ConstantBuffer()
{
	if (_constantBuffer)
		_constantBuffer->Release();
}

ID3D11Buffer** ConstantBuffer::getConstantBuffer()
{
	return &_constantBuffer;
}
