#pragma once

namespace ECS
{
	class Registry;
}
namespace Behemoth
{
	class RigidBodySystem
	{
	public:
		RigidBodySystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);
	private:
		void ApplyGravity();
	};
}

