#pragma once

#include "NextAPI/App/app.h"

class InputManager
{
public:
	InputManager(InputManager& obj) = delete;
	InputManager& operator= (const InputManager& obj) = delete;

	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	// TODO:
	// container for keyboard input tracking
	// container for mouse input tracking
	// container for controller 
	// Need to track previous frame input states as well as current frames input states

	// Some way to track multiple controller inputs
private:
	InputManager() = default;

};

