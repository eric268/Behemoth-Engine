#pragma once
#include "Input/InputCodes.h"
#include "NextAPI/App/app.h"
#include <utility>

namespace Behemoth
{
	class Input
	{
	public:
		static inline bool IsKeyDown(KeyCode code)
		{
			return App::IsKeyPressed(static_cast<int>(code));
		}
		static bool IsMouseButtonDown(MouseCode code);
		static std::pair<float, float> GetMousePosition();
	};
}
