#include "InputController.h"

void InputController::setMouseMode()
{
	// RELATIVE calculates delta pos
	// ABSOLUTE takes screen coordinates (not client)
	if (_mouse->GetState().rightButton)
		_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	else
		_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
}

InputController::InputController()
{
}

InputController::InputController(HWND window)
{
	_keyboard = std::make_unique<DirectX::Keyboard>();
	_mouse = std::make_unique<DirectX::Mouse>();
	_mouse->SetWindow(window);
}

InputController::~InputController()
{
}

void InputController::translateMessage(MSG message)
{
	switch (message.message)
	{
	case WM_KEYDOWN:
		DirectX::Keyboard::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_KEYUP:
		DirectX::Keyboard::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_MOUSEMOVE:
		DirectX::Mouse::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_RBUTTONDOWN:
		DirectX::Mouse::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_RBUTTONUP:
		DirectX::Mouse::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_INPUT:
		DirectX::Mouse::ProcessMessage(message.message, message.wParam, message.lParam);
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(message.message, message.wParam, message.lParam);
		break;
	}
}

DirectX::Mouse::State InputController::getMouseState() const
{
	return _mouse->GetState();
}

DirectX::Keyboard::State InputController::getKeyboardState() const
{
	return _keyboard->GetState();
}

DirectX::GamePad::State InputController::getGamePadState() const
{
	return _gamePad->GetState(1);
}

void InputController::setWindow(HWND window)
{
	_mouse->SetWindow(window);
}
