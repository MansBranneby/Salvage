#include "InputController.h"

InputController::InputController()
{
}

InputController::~InputController()
{
}

void InputController::translateMessage(MSG * message)
{
	switch (message->message)
	{
	case WM_KEYDOWN:
		DirectX::Keyboard::ProcessMessage(message->message, message->wParam, message->lParam);
	case WM_KEYUP:
		DirectX::Keyboard::ProcessMessage(message->message, message->wParam, message->lParam);
	case WM_MOUSEMOVE:
		DirectX::Mouse::ProcessMessage(message->message, message->wParam, message->lParam);
	case WM_RBUTTONDOWN:
		DirectX::Mouse::ProcessMessage(message->message, message->wParam, message->lParam);
	case WM_RBUTTONUP:
		DirectX::Mouse::ProcessMessage(message->message, message->wParam, message->lParam);
	case WM_INPUT:
		DirectX::Mouse::ProcessMessage(message->message, message->wParam, message->lParam);
		break;
	}

	TranslateMessage(message);
	DispatchMessage(message);
}

DirectX::Mouse::State InputController::getMouseState()
{
	return DirectX::Mouse::State();
}

DirectX::Keyboard::State InputController::getKeyboardState()
{
	return DirectX::Keyboard::State();
}

DirectX::GamePad::State InputController::getGamePadState()
{
	return DirectX::GamePad::State();
}
