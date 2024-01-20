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
                collisionData] : registry.Get<TransformComponent, VelocityComponent,  CollisionDataComponent>())
		{
 			BMath::Vector3 offsetPosition;
 			BMath::Vector3 offsetVelocity;
  
  			for (auto& collision : collisionData->data)
  			{
  				offsetPosition += collision.data.collisionNormal * collision.data.depth;
 				float velocityAlongNormal = BMath::Vector3::Dot(velocityComp->velocity, collision.data.collisionNormal);
 				offsetVelocity += collision.data.collisionNormal * -velocityAlongNormal;
  			}
  
 			registry.AddComponent<MoveComponent>(entity, offsetPosition);
			velocityComp->velocity += offsetVelocity * 0.96f;
		}
	}
}