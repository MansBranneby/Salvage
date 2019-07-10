#pragma once

#define NOMINMAX
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
#include "Game.h"
#include "GraphicResources.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputController.h"
#include "Clock.h"
#include "Drawable.h"
#include "Model.h"
#include "GameState.h"

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

// GLOBALS //
Game* gGame = nullptr;
GraphicResources* gGR = nullptr;

// CONTROLLER //
InputController* gInputCtrl;

// CLOCK //
Clock* gClock;
//ID3D11Buffer* constantBuffer; //TILLFÄLLIG
//Model gModel, gOriginObject;

// STATES
GameState gGameState;

// SHADERS //
VertexShader* gVS = nullptr;
PixelShader* gPS = nullptr;

VertexShader* gVSBV = nullptr;
PixelShader* gPSBV = nullptr;

// IMGUI VARIABLES //
float gSmoothSpeed = 1.0f;
float gLookAtSpeed = 1.0f;
bool drawBoundingVolume = false;

void initializeResources(HWND wndHandle)
{
	//GRAPHIC RESOURCES
	gGR = new GraphicResources(wndHandle);

	//SHADERS
	gVS = new VertexShader(L"VertexShader.hlsl", gGR->getDevice());
	gPS = new PixelShader(L"PixelShader.hlsl", gGR->getDevice());
	
	gVSBV = new VertexShader(L"VertexShaderBV.hlsl", gGR->getDevice());
	gPSBV = new PixelShader(L"PixelShaderBV.hlsl", gGR->getDevice());

	//INPUT CONTROLLER
	gInputCtrl = new InputController(wndHandle);

	//Clock
	gClock = new Clock();
	//GAME
	gGame = new Game(gGR->getDevice(), gGR->getDeviceContext(), WIDTH, HEIGHT, gClock, gInputCtrl);
	gGame->pushState(&gGameState); // Change which state the game is in, not done yet.
	//TESTMODEL
	//gModel.loadModel(gGR->getDevice(), gGR->getDeviceContext(), ".\\Resources\\Models\\gubbe1Ani.dae");
	//gOriginObject.loadModel(gGR->getDevice(), gGR->getDeviceContext(), ".\\Resources\\Models\\noani.dae");

	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(wndHandle);
	ImGui_ImplDX11_Init(gGR->getDevice(), gGR->getDeviceContext());
	ImGui::StyleColorsDark();

	//Tillfällig för test av animation
	//gClock.startAnimation();
}

void imGuiUpdate()
{
	gGR->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");
	ImGui::Text("Camera");
	ImGui::SliderFloat("Smoothspeed", &gSmoothSpeed, 0.0f, 20.0f);
	ImGui::SliderFloat("LookAtSpeed", &gLookAtSpeed, 0.0f, 20.0f);
	gGame->getCamera()->setSmoothSpeed(gSmoothSpeed);
	gGame->getCamera()->setLookAtSpeed(gLookAtSpeed);
	ImGui::Text("Player position: X: %.2f, Y: %.2f, Z: %.2f", XMVectorGetX(gGame->getPlayer()->getPosition()), XMVectorGetY(gGame->getPlayer()->getPosition()), XMVectorGetZ(gGame->getPlayer()->getPosition()));
	ImGui::Checkbox("Draw bounding volume", &drawBoundingVolume);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void update()
{
	gGame->handleInput();
	gGame->update();
}


void render()
{
	gGR->getDeviceContext()->VSSetShader(&gVS->getVertexShader(), nullptr, 0);
	gGR->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->PSSetShader(&gPS->getPixelShader(), nullptr, 0);

	gGR->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gGR->getDeviceContext()->IASetInputLayout(&gVS->getvertexLayout());
	gGR->getDeviceContext()->PSSetSamplers(0, 1, gGR->getSamplerState());

	gGame->draw();

	gGR->getDeviceContext()->VSSetShader(&gVSBV->getVertexShader(), nullptr, 0);
	gGR->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->PSSetShader(&gPSBV->getPixelShader(), nullptr, 0);

	gGR->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	gGR->getDeviceContext()->IASetInputLayout(&gVSBV->getvertexLayout());
	gGR->getDeviceContext()->PSSetSamplers(0, 0, nullptr);

	if (drawBoundingVolume == true)
	{
		// Temporary solution to drawing bounding volumes (Drawing these in gamestate does not work)
		gGame->getPlayer()->drawBoundingVolume(gGR->getDeviceContext()); // draw player bounding volume
		// Loop through all the objects in levelhandler and draw their bounding volumes
		for(int i = 0; i < gGame->getLevelHandler()->getNrOfGameObjects(); i++)
			gGame->getLevelHandler()->getGameObject(i)->drawBoundingVolume(gGR->getDeviceContext());
	}
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
		initializeResources(wndHandle); //GR, SHADERS, IMGUI

		ShowWindow(wndHandle, nCmdShow);
		
		//TEST
		gInputCtrl->setWindow(wndHandle);

		///////////////
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				gInputCtrl->translateMessage(msg);

				 //PRESS ESC TO QUIT PROGRAM
				if (gInputCtrl->getKeyboardState().Escape)
					msg.message = WM_QUIT;
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// RENDER //
				gGR->getDeviceContext()->RSSetState(gGR->getRasterizerState());
				float clearColour[] = {0.0f,0.0f,0.0f};

				gGR->getDeviceContext()->ClearRenderTargetView(*gGR->getBackBuffer(), clearColour);
				gGR->getDeviceContext()->OMSetRenderTargets(1, gGR->getBackBuffer(), gGR->getDepthStencilView());//ENABLE DEPTH TEST WHEN WE HAVE A CAMERA
				gGR->getDeviceContext()->ClearDepthStencilView(gGR->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

				update();
				render();
				imGuiUpdate();

				gGR->getSwapChain()->Present(0, 0);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				//Reset clock
				gClock->reset();
			}
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		//CLEAR - kanske i en separat funktion
		delete gGR;
		delete gVS;
		delete gPS;
		delete gInputCtrl;
		delete gGame;
		delete gClock;
		delete gVSBV;
		delete gPSBV;
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