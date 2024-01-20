#include "pch.h"
#include "RigidBodySystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Core/Log.h"

namespace Behemoth
{
	void RigidBodySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [
			entity,
				rigidBodyComp,
				velocityComp] : registry.Get<RigidBodyComponent, VelocityComponent>())
		{
			if (!rigidBodyComp->affectedByGravity)
			{
				continue;
			}

			bool collidingOnBottom = false;
			if (CollisionDataComponent* collisionData = registry.GetComponent<CollisionDataComponent>(entity))
			{
				for (const auto& collisionData : collisionData->data)
				{
					if (collisionData.data.collisionNormal == BMath::Vector3::Up())
					{
						velocityComp->velocity.y -= deltaTime * GRAVITY;
						break;
					}
				}
			}

			// if (!collidingOnBottom)
			{
				velocityComp->velocity.y += deltaTime * GRAVITY;
			}
			
		}
	}
}