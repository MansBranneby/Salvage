#include "GraphicResources.h"

void GraphicResources::initializeResources(HWND wndHandle)
{
	_device = nullptr;
	_deviceContext = nullptr;
	_swapChain = nullptr;
	_rasterizerState = nullptr;
	_depthStencilView = nullptr;
	_backbufferRTV = nullptr;
	_samplerState = nullptr;

	createDirect3DContext(wndHandle);
	createDepthStencil();
	setRasterizerState();
	setSamplerState();
	setViewPort();
	
	_shaderHandler = new ShaderHandler(_device);
}

void GraphicResources::createDepthStencil()
{
	ID3D11Texture2D* pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = (UINT)WIDTH;
	descDepth.Height = (UINT)HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = _device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		MessageBox(NULL, L"pDepthStencil", L"Error", MB_OK | MB_ICONERROR);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	// Depth test parameters
	dsDesc.DepthEnable = true; //SET TRUE WHEN WE HAVE A CAMERA
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	//dsDesc.StencilEnable = true;
	//dsDesc.StencilReadMask = 0xFF;
	//dsDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	hr = _device->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr))
		MessageBox(NULL, L"pDSState", L"Error", MB_OK | MB_ICONERROR);

	// Bind depth stencil state
	_deviceContext->OMSetDepthStencilState(pDSState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	hr = _device->CreateDepthStencilView(pDepthStencil, &descDSV, &_depthStencilView);
	if (FAILED(hr))
		MessageBox(NULL, L"_depthStencilView", L"Error", MB_OK | MB_ICONERROR);

	pDepthStencil->Release();
	pDSState->Release();
}

void GraphicResources::setViewPort()
{
	_viewPort.Width = (UINT)WIDTH;
	_viewPort.Height = (UINT)HEIGHT;
	_viewPort.MinDepth = 0.0f;
	_viewPort.MaxDepth = 1.0f;
	_viewPort.TopLeftX = 0;
	_viewPort.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &_viewPort);
}

void GraphicResources::setRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	HRESULT hr = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
	if (FAILED(hr))
		MessageBox(NULL, L"_rasterizerState", L"Error", MB_OK | MB_ICONERROR);
}

void GraphicResources::setSamplerState()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = _device->CreateSamplerState(&sampDesc, &_samplerState);
	if (FAILED(hr))
		MessageBox(NULL, L"_samplerState", L"Error", MB_OK | MB_ICONERROR);
}

HRESULT GraphicResources::createDirect3DContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferDesc.Width = (UINT)WIDTH;
	scd.BufferDesc.Height = (UINT)HEIGHT;
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&_swapChain,
		&_device,
		NULL,
		&_deviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		//_swapChain->SetFullscreenState(false, NULL); //FULLSCREEN ON/OFF
		// use the back buffer address to create the render target
		_device->CreateRenderTargetView(pBackBuffer, NULL, &_backbufferRTV);
		pBackBuffer->Release();
	}
	return hr;
}

GraphicResources::GraphicResources()
{
}

GraphicResources::GraphicResources(HWND wndHandle)
{
	initializeResources(wndHandle);
}

GraphicResources::~GraphicResources()
{
	if(_device)
		_device->Release();
	if(_deviceContext)
		_deviceContext->Release();
	if(_swapChain)
		_swapChain->Release();
	if(_rasterizerState)
		_rasterizerState->Release();
	if(_depthStencilView)
		_depthStencilView->Release();
	if(_backbufferRTV)
		_backbufferRTV->Release();
	if (_samplerState)
		_samplerState->Release();
	if (_shaderHandler)
		delete _shaderHandler;
}

ID3D11Device * GraphicResources::getDevice()
{
	return _device;
}

ID3D11DeviceContext * GraphicResources::getDeviceContext() const
{
	return _deviceContext;
}

IDXGISwapChain * GraphicResources::getSwapChain() const
{
	return _swapChain;
}

ID3D11RasterizerState * GraphicResources::getRasterizerState() const
{
	return _rasterizerState;
}

ID3D11DepthStencilView * GraphicResources::getDepthStencilView() const
{
	return _depthStencilView;
}

ID3D11RenderTargetView ** GraphicResources::getBackBuffer()
{
	return &_backbufferRTV;
}

ID3D11SamplerState ** GraphicResources::getSamplerState()
{
	return &_samplerState;
}

ShaderHandler * GraphicResources::getShaderHandler()
{
	return _shaderHandler;
}
