 #include "pch.h"
#include "CollisionResolutionSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Physics/Collision/Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{
	void CollisionResolutionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [
			entityHandle,
				transformComp,
				velocityComp,
				rigidBodyComp,
				collisionData] : registry.Get<TransformComponent, VelocityComponent, RigidBodyComponent, CollisionDataComponent>())
		{
			BMath::Vector3 offsetPosition = BMath::Vector3::Zero();
			BMath::Vector3 newVelocity = velocityComp->velocity;
			bool simPhysics = rigidBodyComp->simulatePhysics;

			for (auto& collision : collisionData->data)
			{
				offsetPosition += collision.data.collisionNormal * collision.data.depth;

				float velocityAlongNormal = BMath::Vector3::Dot(newVelocity, collision.data.collisionNormal);
				if (velocityAlongNormal < 0) 
				{
					float bounce = (simPhysics) ? velocityAlongNormal * collision.data.physicsMaterial.restitution : 0.0f;
					newVelocity -= collision.data.collisionNormal * (velocityAlongNormal + bounce);
				}

				if (simPhysics)
				{
					newVelocity *= (1.0f - collision.data.physicsMaterial.dampening * deltaTime);
				}
			}

			velocityComp->velocity = newVelocity;

			if (MoveComponent* moveComp = registry.GetComponent<MoveComponent>(entityHandle))
			{
				moveComp->location += offsetPosition;
			}
			else
			{
				registry.AddComponent<MoveComponent>(entityHandle, offsetPosition);
			}
		}
	}
}