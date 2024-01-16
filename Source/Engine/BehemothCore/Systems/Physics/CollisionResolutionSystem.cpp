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
		for (const auto& [entity, transformComp, velocityComp, collisionData] : registry.Get<TransformComponent, VelocityComponent,  CollisionDataComponent>())
		{
			
/*			LOGMESSAGE(General, "Size: " + collisionData->data.size());*/
 			BMath::Vector3 offsetPosition;
 			BMath::Vector3 offsetVelocity;
  
  			for (auto& collision : collisionData->data)
  			{
// 				LOGMESSAGE(Warning, "Normal: " + collision.data.collisionNormal.Print());
// 				LOGMESSAGE(Warning, "Pen: " + std::to_string(collision.data.penetrationDepth));

  				offsetPosition += collision.data.collisionNormal * collision.data.depth;
 
 				float velocityAlongNormal = BMath::Vector3::Dot(velocityComp->velocity, collision.data.collisionNormal);
 				offsetVelocity += collision.data.collisionNormal * -velocityAlongNormal;
  			}
  
 			registry.AddComponent<MoveComponent>(entity, offsetPosition);
			velocityComp->velocity += offsetVelocity * 0.99f;
		}
	}
}