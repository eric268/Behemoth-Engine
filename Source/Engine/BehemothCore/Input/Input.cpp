#include "pch.h"
#include "Input.h"
#include "Event/Event.h"
#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"
#include "InputCodes.h"
#include "InputMapping.h"
#include "NextAPI/App/SimpleController.h"

namespace Behemoth
{
	// Initialize the static members of Input.
	std::bitset<NUM_KC> Input::currentKeyState;
	std::bitset<NUM_KC> Input::prevKeyState;

	std::bitset<NUM_MC> Input::currentMouseState;
	std::bitset<NUM_MC> Input::prevMouseState;

	std::vector<std::bitset<NUM_CC>> Input::currentControllerButtonState(MAX_CONTROLLERS);
	std::vector<std::bitset<NUM_CC>> Input::prevControllerButtonState(MAX_CONTROLLERS);

	std::pair<float, float> Input::mousePosition = std::make_pair(0.0f, 0.0f);

	void Input::Update(const float deltaTime)
	{
		prevKeyState = currentKeyState;
		prevMouseState = currentMouseState;
		prevControllerButtonState = currentControllerButtonState;

		ProcessControllerInput();
	}

	void Input::ProcessControllerInput()
	{
		for (int controller = 0; controller < MAX_CONTROLLERS; controller++)
		{
			for (int button = 0; button < Behemoth::ControllerCode::NUM_CC; button++)
			{
				currentControllerButtonState[controller][button] = CSimpleControllers::GetInstance().GetController(controller).CheckButton(Behemoth::controllerButtonMap[button], false);
			}
		}
	}

	bool Input::OnEvent(Event& event)
	{
		EventHandler eventDispatcher{ event };

 		if (eventDispatcher.ProcessEvent<KeyDownEvent>(&Input::OnKeyDown)			|| 
 			eventDispatcher.ProcessEvent<KeyReleasedEvent>(&Input::OnKeyReleased)  ||
 			eventDispatcher.ProcessEvent<MouseDownEvent>(&Input::OnMouseDown)		||
 			eventDispatcher.ProcessEvent<MouseUpEvent>(&Input::OnMouseUp)			||
 			eventDispatcher.ProcessEvent<MouseMoveEvent>(&Input::OnMouseMove))
			return true;

		return false;
	}

	bool Input::IsAnyKeyDown()
	{
		return !currentKeyState.none();
	}

	bool Input::IsKeyDown(KeyCode code)
	{
		return !prevKeyState.test(code) && currentKeyState.test(code);
	}

	bool Input::IsKeyReleased(KeyCode code)
	{
		return prevKeyState.test(code) && !currentKeyState.test(code);
	}

	bool Input::IsKeyHeld(KeyCode code)
	{
		return prevKeyState.test(code) && currentKeyState.test(code);
	}

	bool Input::IsMouseButtonDown(MouseCode code)
	{
		return currentMouseState.test(code);
	}

	bool Input::IsMouseButtonReleased(MouseCode code)
	{
		return prevMouseState.test(code) && !currentMouseState.test(code);
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return mousePosition;
	}

	void Input::OnKeyDown(const KeyDownEvent& event)
	{
		const KeyCode code = event.GetKeyCode();
		currentKeyState.set(code, 1);
	}

	void Input::OnKeyReleased(const KeyReleasedEvent& event)
	{
		const KeyCode code = event.GetKeyCode();
		currentKeyState.set(code, 0);
	}

	void Input::OnMouseDown(const MouseDownEvent& event)
	{
		const MouseCode code = event.GetMouseCode();
		currentMouseState.set(code, 1);
	}

	void Input::OnMouseUp(const MouseUpEvent& event)
	{
		const MouseCode code = event.GetMouseCode();
		currentMouseState.set(code, 0);
	}

	void Input::OnMouseMove(const MouseMoveEvent& event)
	{
		mousePosition = event.GetMousePos();
	}

	bool Input::IsControllerKeyDown(ControllerCode code, int controller)
	{
		return !prevControllerButtonState[controller][code] && currentControllerButtonState[controller][code];
	}
	bool Input::IsControllerKeyHeld(ControllerCode code, int controller)
	{
		return prevControllerButtonState[controller][code] && currentControllerButtonState[controller][code];
	}
	bool Input::IsControllerKeyUp(ControllerCode code, int controller)
	{
		return prevControllerButtonState[controller][code] && !currentControllerButtonState[controller][code];
	}

	AnalogInput Input::GetLeftControllerAnalog(int controller)
	{
		float x = CSimpleControllers::GetInstance().GetController(controller).GetLeftThumbStickX();
		float y = CSimpleControllers::GetInstance().GetController(controller).GetLeftThumbStickY();
		return {x, y};
	}

	AnalogInput Input::GetRightControllerAnalog(int controller)
	{
		float x = CSimpleControllers::GetInstance().GetController(controller).GetRightThumbStickX();
		float y = CSimpleControllers::GetInstance().GetController(controller).GetRightThumbStickY();
		return {x, y};
	}

	float Input::GetLeftControllerTrigger(int controller)
	{
		return CSimpleControllers::GetInstance().GetController(controller).GetLeftTrigger();
	}
	float Input::GetRightControllerTrigger(int controller)
	{
		return CSimpleControllers::GetInstance().GetController(controller).GetRightTrigger();
	}
}