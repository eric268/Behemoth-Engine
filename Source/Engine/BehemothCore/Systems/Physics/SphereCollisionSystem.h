#pragma once

#include "Math/MathCore.h"
#include "ECS/ECSCore.h"

namespace Behemoth
{


	class SphereCollisionSystem
	{
	public:
		SphereCollisionSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);

	private:

	};
}
