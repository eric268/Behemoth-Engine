#pragma once
#include "Input/InputCodes.h"
#include "NextAPI/App/app.h"
#include <utility>
#include <bitset>

namespace Behemoth
{
	class Event;
	class KeyDownEvent;
	class KeyReleasedEvent;
	class MouseDownEvent;
	class MouseUpEvent;
	class MouseMoveEvent;

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode code);
		static bool IsKeyReleased(KeyCode code);
		static bool IsKeyHeld(KeyCode code);

		static bool IsMouseButtonDown(MouseCode code);
		static bool IsMouseButtonReleased(MouseCode code);
		static std::pair<float, float> GetMousePosition();

		static void Update(const float deltaTime);
		static bool OnEvent(Event& event);

	private:
		void static OnKeyDown(const KeyDownEvent& event);
		void static OnKeyReleased(const KeyReleasedEvent& event);
		void static OnMouseDown(const MouseDownEvent& event);
		void static OnMouseUp(const MouseUpEvent& event);
		void static OnMouseMove(const MouseMoveEvent& event);

		static std::bitset<NUM_KEYS> currentKeyState;
		static std::bitset<NUM_KEYS> prevKeyState;

		static std::bitset<NUM_MOUSE_KEYS> currentMouseState;
		static std::bitset<NUM_MOUSE_KEYS> prevMouseState;

		static std::pair<float, float> mousePosition;
	};
}
