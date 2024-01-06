#include "pch.h"
#include "RigidBodySystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void RigidBodySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, rigidBodyComp, velocityComp] : registry.Get<RigidBodyComponent, VelocityComponent>())
		{
			velocityComp->velocity.y += deltaTime * GRAVITY;
		}
	}
}