#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(ECS::Registry& registry);
	};
}
