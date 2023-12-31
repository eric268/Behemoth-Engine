#pragma once
#include "Input/InputCodes.h"
#include "NextAPI/App/app.h"
#include <utility>
#include <bitset>

namespace Behemoth
{
	class Event;

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode code);
		static bool IsKeyUp(KeyCode code);
		static bool IsKeyHeld(KeyCode code);




		static bool IsMouseButtonDown(MouseCode code);
		static std::pair<float, float> GetMousePosition();

		void OnEvent(Event& event);

	private:

		std::bitset<NUM_KEYS> currentKeyState;
		std::bitset<NUM_KEYS> prevKeyState;
	};
}
