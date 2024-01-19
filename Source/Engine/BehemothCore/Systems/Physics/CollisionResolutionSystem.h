#pragma once

#include "ECS/Entity.h"


namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct CollisionResolutionSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& regsitry);
	};
}