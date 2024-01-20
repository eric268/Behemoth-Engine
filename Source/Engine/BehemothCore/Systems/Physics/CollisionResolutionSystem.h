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
		void Run(const float deltaTime, ECS::Registry& regsitry);
	};
}