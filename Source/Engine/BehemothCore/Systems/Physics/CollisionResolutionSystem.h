#pragma once

#include "ECS/Entity.h"


namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class CollisionResolutionSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& regsitry);

	private:

	};
}