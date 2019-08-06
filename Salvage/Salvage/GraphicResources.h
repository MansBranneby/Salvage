#pragma once
#include "assimp/scene.h"
#include <d3d11.h>
#include "ShaderHandler.h"
#include "ConstantBuffer.h"
#define WIDTH 1920.0f
#define HEIGHT 1080.0f

struct PerFrameData
{
	DirectX::XMMATRIX VP;
	DirectX::XMVECTOR camPos;
	float minTess;
	float maxTess;
	float minDist;
	float maxDist;
	DirectX::XMFLOAT2 texScale;
	DirectX::XMFLOAT2 padding;
};

struct PerObjectData
{
	aiMatrix4x4 world;
};

class GraphicResources
{
private:
	void initializeResources(HWND wndHandle);
	void createDepthStencil();
	void setViewPort();
	void setRasterizerState();
	void setSamplerState();
	void setConstantBuffers();
	HRESULT createDirect3DContext(HWND wndHandle);

	D3D11_VIEWPORT _viewPort; //Min gissning är att denna behövs utanför denna klass
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	IDXGISwapChain* _swapChain;
	ID3D11RasterizerState* _rasterizerState;
	
	// VIEWS //
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RenderTargetView* _backbufferRTV;
	
	// SAMPLERS //
	ID3D11SamplerState* _samplerState = nullptr;

	// CONSTANTBUFFERS //
	PerFrameData _perFrameData;
	PerObjectData _perObjectData;
	ConstantBuffer* _perFrameCB;
	ConstantBuffer* _perObjectCB;

	// SHADERS //
	ShaderHandler* _shaderHandler;

public:
	
	// Member functions
	GraphicResources();
	GraphicResources(HWND wndHandle);
	~GraphicResources();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext() const;
	IDXGISwapChain* getSwapChain() const;
	ID3D11RasterizerState* getRasterizerState() const;
	ID3D11DepthStencilView* getDepthStencilView() const;
	ID3D11RenderTargetView** getBackBuffer();
	ID3D11SamplerState** getSamplerState();
	ShaderHandler* getShaderHandler();

	PerFrameData* getPerFrameData();
	PerObjectData* getPerObjectData();

	void updatePerFrameCB();
	void updatePerObjectCB();
};