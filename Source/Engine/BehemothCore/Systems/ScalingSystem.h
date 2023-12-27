#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class ScalingSystem
	{
	public:
		ScalingSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

