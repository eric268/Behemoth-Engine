#pragma once

namespace ECS
{
	class Registry;
}

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