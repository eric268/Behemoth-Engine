#pragma once

namespace ECS
{
	class Registry;
}

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
