#pragma once

namespace ECS
{
	class Registry;
}
namespace Behemoth
{
	struct RigidBodySystem
	{
	public:
		RigidBodySystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

