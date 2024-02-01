#pragma once

namespace ECS
{
	class Registry;
}
namespace Behemoth
{
	struct RigidBodySystem
	{
		void Run(const float deltaTime, ECS::Registry& registry);
	};
}

