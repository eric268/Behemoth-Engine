#pragma once
#include "Input/InputCodes.h"
#include "NextAPI/App/app.h"

#include <utility>
#include <bitset>
#include <vector>

#define MAX_CONTROLLERS 4
#define TRIGGER_DEADZONE 0.3

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
		static bool IsAnyKeyDown();

		static bool IsKeyDown(KeyCode code);
		static bool IsKeyReleased(KeyCode code);
		static bool IsKeyHeld(KeyCode code);

		static bool IsMouseButtonDown(MouseCode code);
		static bool IsMouseButtonReleased(MouseCode code);
		static std::pair<float, float> GetMousePosition();

		static bool IsControllerKeyDown(ControllerCode code, int controller = 0);
		static bool IsControllerKeyHeld(ControllerCode code, int controller = 0);
		static bool IsControllerKeyUp(ControllerCode code, int controller = 0);

		static AnalogInput GetLeftControllerAnaloge(int controller = 0);
		static AnalogInput GetRightControllerAnaloge(int controller = 0);

		static float GetLeftControllerTrigger(int controller = 0);
		static float GetRightControllerTrigger(int controller = 0);

		static void Update(const float deltaTime);
		static bool OnEvent(Event& event);

	private:
		// Declare within Input to keep type saftey
		static void OnKeyDown(const KeyDownEvent& event);
		static void OnKeyReleased(const KeyReleasedEvent& event);
		static void OnMouseDown(const MouseDownEvent& event);
		static void OnMouseUp(const MouseUpEvent& event);
		static void OnMouseMove(const MouseMoveEvent& event);

		static void ProcessControllerInput();

		static std::bitset<NUM_KC> currentKeyState;
		static std::bitset<NUM_KC> prevKeyState;

		static std::bitset<NUM_MC> currentMouseState;
		static std::bitset<NUM_MC> prevMouseState;

		static std::vector<std::bitset<NUM_CC>> currentControllerButtonState;
		static std::vector<std::bitset<NUM_CC>> prevControllerButtonState;

		static std::pair<float, float> mousePosition;
	};
}
