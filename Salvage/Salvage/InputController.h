#pragma once
#include <iostream>
#include <DirectXMath.h>

#include <windows.h>

// DirectXTK
#include "Keyboard.h"
#include "Mouse.h"
#include "GamePad.h"

class InputController
{
private:
	std::unique_ptr<DirectX::GamePad> _gamePad;
	std::unique_ptr<DirectX::Keyboard> _keyboard;
	std::unique_ptr<DirectX::Mouse> _mouse;

public:
	InputController();
	~InputController();

	void translateMessage(MSG message);

	DirectX::Mouse::State getMouseState() const;
	DirectX::Keyboard::State getKeyboardState() const;
	DirectX::GamePad::State getGamePadState() const;
};