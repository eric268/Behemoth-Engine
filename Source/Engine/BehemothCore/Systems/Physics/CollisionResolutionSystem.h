#pragma once

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