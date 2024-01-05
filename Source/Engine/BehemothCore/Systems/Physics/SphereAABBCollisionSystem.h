#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class SphereAABBCollisionSystem
	{
	public:
		SphereAABBCollisionSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);
	private:
	};
}
