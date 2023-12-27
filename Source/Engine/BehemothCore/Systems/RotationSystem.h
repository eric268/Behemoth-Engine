#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);
	};
}
