#include "pch.h"
#include "Input.h"
#include "Event/Event.h"
#include "Event/KeyboardEvents.h"
#include "Event/MouseEvents.h"

namespace Behemoth
{
#include "Input.h" // Or whatever the name of your header file is

	// Initialize the static members of Input.
	std::bitset<NUM_KEYS> Input::currentKeyState;
	std::bitset<NUM_KEYS> Input::prevKeyState;

	std::bitset<NUM_MOUSE_KEYS> Input::currentMouseState;
	std::bitset<NUM_MOUSE_KEYS> Input::prevMouseState;

	std::pair<float, float> Input::mousePosition = std::make_pair(0.0f, 0.0f);

	void Input::Update(const float deltaTime)
	{
		prevKeyState = currentKeyState;
		prevMouseState = currentMouseState;
	}

	bool Input::OnEvent(Event& event)
	{
		EventDispatcher eventDispatcher{ event };

 		if (eventDispatcher.Dispatch<KeyDownEvent>(&Input::OnKeyDown)			|| 
 			eventDispatcher.Dispatch<KeyReleasedEvent>(&Input::OnKeyReleased)  ||
 			eventDispatcher.Dispatch<MouseDownEvent>(&Input::OnMouseDown)		||
 			eventDispatcher.Dispatch<MouseUpEvent>(&Input::OnMouseUp)			||
 			eventDispatcher.Dispatch<MouseMoveEvent>(&Input::OnMouseMove))
			return true;

		return false;
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
		return currentKeyState.test(code) && prevKeyState.test(code);
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
		// std::cout << "Key Down\n";

		const KeyCode code = event.GetKeyCode();
		// prevKeyState.set(code, currentKeyState.test(code));
		currentKeyState.set(code, 1);
	}

	void Input::OnKeyReleased(const KeyReleasedEvent& event)
	{
		// std::cout << "Key Released\n";
		const KeyCode code = event.GetKeyCode();
		// prevKeyState.set(code, currentKeyState.test(code));
		currentKeyState.set(code, 0);
	}

	void Input::OnMouseDown(const MouseDownEvent& event)
	{
		const MouseCode code = event.GetMouseCode();
		// prevMouseState.set(code, currentMouseState.test(code));
		currentMouseState.set(code, 1);
	}

	void Input::OnMouseUp(const MouseUpEvent& event)
	{
		const MouseCode code = event.GetMouseCode();
		// prevMouseState.set(code, currentMouseState.test(code));
		currentMouseState.set(code, 0);
	}

	void Input::OnMouseMove(const MouseMoveEvent& event)
	{
		mousePosition = event.GetMousePos();
	}
}