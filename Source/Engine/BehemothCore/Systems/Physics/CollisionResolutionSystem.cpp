 #include "pch.h"
#include "CollisionResolutionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{
	void CollisionResolutionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [
			entity,
				transformComp,
				velocityComp,
				collisionData] : registry.Get<TransformComponent, VelocityComponent, CollisionDataComponent>())
		{
			BMath::Vector3 offsetPosition;
			BMath::Vector3 newVelocity = velocityComp->velocity;

			for (auto& collision : collisionData->data)
			{
			//	offsetPosition += collision.data.collisionNormal * collision.data.depth;

				float velocityAlongNormal = BMath::Vector3::Dot(newVelocity, collision.data.collisionNormal);
				if (velocityAlongNormal < 0) 
				{
					float bounce = velocityAlongNormal * collision.data.physicsMaterial.restitution;
					newVelocity -= collision.data.collisionNormal * (velocityAlongNormal + bounce);
				}

				newVelocity *= (1.0f - collision.data.physicsMaterial.dampening * deltaTime);
			}



			registry.AddComponent<MoveComponent>(entity, offsetPosition);
			velocityComp->velocity = newVelocity;
		}
	}
}