#pragma once
#include "ECS/ECSCore.h"

namespace Behemoth
{
	class AABBCollisionSystem
	{
	public:
		AABBCollisionSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
	};
}