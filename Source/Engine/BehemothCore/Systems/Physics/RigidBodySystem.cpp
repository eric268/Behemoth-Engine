#include "pch.h"
#include "RigidBodySystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth
{
	void RigidBodySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<RigidBodyComponent, VelocityComponent>();
	}

	void RigidBodySystem::ApplyGravity()
	{

	}
}