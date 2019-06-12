#include "VertexShader.h"

// Help function to create VertexBuffer
HRESULT VertexShader::createVertexShader(LPCWSTR fileName, ID3D11Device * device, ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	HRESULT result = D3DCompileFromFile(fileName, nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_DEBUG, 0, pVS, errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)(*errorBlob)->GetBufferPointer());
			(*errorBlob)->Release();
		}
		if (pVS)
			(*pVS)->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createVertexShader in VertexShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	device->CreateVertexShader((*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), nullptr, &_vertexShader);
	return result;
}

void VertexShader::createInputLayout(ID3D11Device* device, ID3DBlob** pVS, ID3DBlob** errorBlob)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",		// "semantic" name in shader
			0,				// "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"TEXCOORD",
			0,				// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,							// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), (*pVS)->GetBufferPointer(), (*pVS)->GetBufferSize(), &_vertexLayout);

	(*pVS)->Release();
}

VertexShader::VertexShader()
{
}

// Constructor
VertexShader::VertexShader(LPCWSTR fileName, ID3D11Device* device)
{
	ID3DBlob* pVS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	createVertexShader(fileName, device, &pVS, &errorBlob);
	createInputLayout(device, &pVS, &errorBlob);

	// Release "refence" to pVS and errorBlob interface object
	if(pVS) pVS->Release();
	if (errorBlob) errorBlob->Release();
}

ID3D11VertexShader & VertexShader::getVertexShader() const
{
	return *_vertexShader;
}

ID3D11InputLayout & VertexShader::getvertexLayout() const
{
	return *_vertexLayout;
}
