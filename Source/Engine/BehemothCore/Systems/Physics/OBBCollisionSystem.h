#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class OBBCollisionSystem
	{
	public:
		OBBCollisionSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);
	};
}