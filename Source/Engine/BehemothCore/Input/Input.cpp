#include "pch.h"
#include "Input.h"

namespace Behemoth
{
	bool Input::IsKeyDown(KeyCode code)
	{
		return false;
	}

	bool Input::IsKeyUp(KeyCode code)
	{
		return false;
	}

	bool Input::IsKeyHeld(KeyCode code)
	{
		return false;
	}

	bool IsMouseButtonDown(MouseCode code)
	{
		return false;
	}

	std::pair<float, float> GetMousePosition()
	{
		return {};
	}

	void OnEvent(Event& event)
	{

	}
}