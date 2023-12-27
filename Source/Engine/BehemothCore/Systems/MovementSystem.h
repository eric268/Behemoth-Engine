#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class MovementSystem
	{
	public:
		MovementSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

