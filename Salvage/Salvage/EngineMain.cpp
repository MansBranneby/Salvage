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

//Behövs dessa kopior. Finns i GraphicsResources
#define WIDTH 1920.0f
#define HEIGHT 1080.0f

// SHADERS //

// GLOBALS //
GraphicResources gGR;

void initializeResources(HWND wndHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // (void)io;
	ImGui_ImplWin32_Init(wndHandle);
	ImGui_ImplDX11_Init(gGR.getDevice(), gGR.getDeviceContext);
	ImGui::StyleColorsDark();
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
	gGR.getDeviceContext()->VSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	gGR.getDeviceContext()->PSSetShader(nullptr, nullptr, 0);

	UINT32 vertexSize = sizeof(/*VERTEXSIZE*/);
	UINT32 offset = 0;

	gGR.getDeviceContext()->IASetVertexBuffers(0, 1, /*VERTEXBUFFER*/, &vertexSize, &offset);
	gGR.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gGR.getDeviceContext()->IASetInputLayout(/*INPUTLAYOUT*/);

	gGR.getDeviceContext()->PSSetSamplers(0, 1, gGR.getSamplerState());

	gGR.getDeviceContext()->Draw(0, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#define _ITERATOR_DEBUG_LEVEL 0
#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); // Skapa fönster

	if (wndHandle)
	{
		// setup textures meshes shaders triangledata constantbuffers etc.
		initializeResources(wndHandle);

		ShowWindow(wndHandle, nCmdShow);

		///////////////
		while (WM_QUIT != msg.message)
		{
			// RENDER //
			gGR.getDeviceContext()->RSSetState(gGR.getRasterizerState());
			float clearColour[3] = {};
			clearColour[3] = 1.0;

			gGR.getDeviceContext()->ClearRenderTargetView(*gGR.getBackBuffer(), clearColour);
			gGR.getDeviceContext()->OMSetRenderTargets(1, gGR.getBackBuffer(), gGR.getDepthStencilView());

			render();
			update();

			gGR.getSwapChain()->Present(0, 0);

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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