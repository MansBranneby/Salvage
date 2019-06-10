#include "InputController.h"

InputController::InputController()
{
	_keyboard = std::make_unique<DirectX::Keyboard>();
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
		break;
	}
}

DirectX::Mouse::State InputController::getMouseState()
{
	return _mouse->GetState();
}

DirectX::Keyboard::State InputController::getKeyboardState()
{
	return _keyboard->GetState();
}

DirectX::GamePad::State InputController::getGamePadState()
{
	return _gamePad->GetState(1);
}
