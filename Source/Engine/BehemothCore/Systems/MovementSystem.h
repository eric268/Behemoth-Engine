#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class MovementSystem
	{
	public:
		MovementSystem() = default;

		void Run(ECS::Registry& registry);
	};
}

