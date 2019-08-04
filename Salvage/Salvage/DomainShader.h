#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class DomainShader
{
private:
	ID3D11DomainShader* _domainShader;
	HRESULT createDomainShader(LPCWSTR filename, ID3D11Device* device);

public:
	DomainShader(LPCWSTR filename, ID3D11Device* device);
	~DomainShader();
	ID3D11DomainShader & getDomainShader();
};