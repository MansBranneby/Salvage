#pragma once

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// Extra

// Own classes
#include "GraphicResources.h"
#include "VertexShader.h"
#include "PixelShader.h"

// DirectXTK
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Beh�vs dessa kopior. Finns i GraphicsResources
#define WIDTH 768.0f
#define HEIGHT 768.0f

// GLOBALS //
GraphicResources gGR;

// SHADERS //
VertexShader gVS;
PixelShader gPS;

ID3D11Buffer* _vertexBuffer = nullptr;
struct PosCol
{
	float x, y, z;
	float r, g, b;
};

PosCol vertexData[3]
{
	0.0f, 0.5f, 0.0f, 
	1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.0f,
	0.0f, 0.0f, 1.0f,

	0.5f, -0.5f, 0.0f,
	0.0f, 1.0f, 0.0f
};

void initializeResources(HWND wndHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // (void)io;
	ImGui_ImplWin32_Init(wndHandle);
	ImGui_ImplDX11_Init(gGR.getDevice(), gGR.getDeviceContext());
	ImGui::StyleColorsDark();


	// VERTEX BUFFER
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;

	bufferDesc.ByteWidth = sizeof(vertexData);
	data.pSysMem = vertexData;

	HRESULT result = gGR.getDevice()->CreateBuffer(&bufferDesc, &data, &_vertexBuffer);
	if (FAILED(result))
		MessageBox(NULL, L"ERROR _vertexBuffer in Mesh.cpp", L"Error", MB_OK | MB_ICONERROR);
}

void imGuiUpdate()
{
	gGR.getDeviceContext()->GSSetShader(nullptr, nullptr, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void update()
{

}

void render()
{
	gGR.getDeviceContext()->VSSetShader(&gVS.getVertexShader(), nullptr, 0);
	gGR.getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->PSSetShader(&gPS.getPixelShader(), nullptr, 0);

	UINT32 vertexSize = sizeof(float)*6;
	UINT32 offset = 0;

	gGR.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &vertexSize, &offset);
	gGR.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gGR.getDeviceContext()->IASetInputLayout(&gVS.getvertexLayout());

	//gGR.getDeviceContext()->PSSetSamplers(0, 1, gGR.getSamplerState());

	gGR.getDeviceContext()->Draw(3, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#define _ITERATOR_DEBUG_LEVEL 0
	#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); // Skapa f�nster

	if (wndHandle)
	{
		gGR = GraphicResources(wndHandle);
		gVS = VertexShader(L"VertexShader.hlsl", gGR.getDevice());
		gPS = PixelShader(L"PixelShader.hlsl", gGR.getDevice());
		// setup textures meshes shaders triangledata constantbuffers etc.
		initializeResources(wndHandle);

		ShowWindow(wndHandle, nCmdShow);

		///////////////
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// RENDER //
				gGR.getDeviceContext()->RSSetState(gGR.getRasterizerState());
				float clearColour[3] = {};
				clearColour[3] = 1.0;

				gGR.getDeviceContext()->ClearRenderTargetView(*gGR.getBackBuffer(), clearColour);
				gGR.getDeviceContext()->OMSetRenderTargets(1, gGR.getBackBuffer(), gGR.getDepthStencilView());//ENABLE DEPTH TEST WHEN WE HAVE A CAMERA

				render();
				update();
				imGuiUpdate();

				gGR.getSwapChain()->Present(0, 0);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		}
	}
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, (int)WIDTH, (int)HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// check if IMGUI can handle the message (when we click INSIDE ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}