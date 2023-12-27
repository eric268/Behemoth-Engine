#pragma once
#include "ECS/ECSCore.h"

namespace Behemoth
{
	class VelocitySystem
	{
	public:
		VelocitySystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
	};

}
