#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class VertexShader
{
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11InputLayout* _vertexLayout;

	// Creates VertexBuffer, this is called upon in the Constructor
	HRESULT createVertexShader(LPCWSTR fileName, ID3D11Device* device, ID3DBlob** pVS, ID3DBlob** errorBlob);
	void createInputLayout(ID3D11Device* device, ID3DBlob** pVS, ID3DBlob** errorBlob);

public:
	VertexShader();
	VertexShader(LPCWSTR fileName, ID3D11Device* device);

	ID3D11VertexShader & getVertexShader() const;
	ID3D11InputLayout & getvertexLayout() const;
};