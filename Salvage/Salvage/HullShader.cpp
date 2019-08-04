#include "HullShader.h"

HRESULT HullShader::createHullShader(LPCWSTR fileName, ID3D11Device* device)
{
	//create pixel shader
	ID3DBlob* pHS = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(fileName, nullptr, nullptr, "HS_main", "hs_5_0", D3DCOMPILE_DEBUG, 0, &pHS, &pErrorBlob);
	// compilation failed?
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pHS)
			pHS->Release();

		if (FAILED(hr))
			MessageBox(NULL, L"Error", L"Error: createPixelShader in pixelShader.cpp has failed", MB_OK | MB_ICONERROR);
	}

	// Compilation successful
	device->CreateHullShader(pHS->GetBufferPointer(), pHS->GetBufferSize(), nullptr, &_hullShader);

	return hr;
}

HullShader::HullShader(LPCWSTR fileName, ID3D11Device * device)
{
	createHullShader(fileName, device);
}

HullShader::~HullShader()
{
	if(_hullShader)
		_hullShader->Release();
}

ID3D11HullShader & HullShader::getHullShader()
{
	return *_hullShader;
}
