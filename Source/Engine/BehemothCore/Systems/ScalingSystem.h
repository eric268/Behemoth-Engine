#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class ScalingSystem
	{
	public:
		ScalingSystem() = default;

		void Run(ECS::Registry& registry);
	};
}

