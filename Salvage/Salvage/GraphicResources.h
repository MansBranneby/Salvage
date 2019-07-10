#pragma once
#include <d3d11.h>

#define WIDTH 1920.0f
#define HEIGHT 1080.0f

class GraphicResources
{
private:
	void initializeResources(HWND wndHandle);
	void createDepthStencil();
	void setViewPort();
	void setRasterizerState();
	void setSamplerState();
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

};