#include "DomainShader.h"

HRESULT DomainShader::createDomainShader(LPCWSTR filename, ID3D11Device * device)
{
	//create pixel shader
	ID3DBlob* pDS = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(filename, nullptr, nullptr, "DS_main", "ds_5_0", D3DCOMPILE_DEBUG, 0, &pDS, &pErrorBlob);
	// compilation failed?
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pDS)
			pDS->Release();

		if (FAILED(hr))
			MessageBox(NULL, L"Error", L"Error: createPixelShader in pixelShader.cpp has failed", MB_OK | MB_ICONERROR);
	}

	// Compilation successful
	device->CreateDomainShader(pDS->GetBufferPointer(), pDS->GetBufferSize(), nullptr, &_domainShader);

	return hr;
}

DomainShader::DomainShader(LPCWSTR filename, ID3D11Device * device)
{
	createDomainShader(filename, device);
}

DomainShader::~DomainShader()
{
	if(_domainShader)
		_domainShader->Release();
}

ID3D11DomainShader & DomainShader::getDomainShader()
{
	return *_domainShader;
}
