#include "pch.h"
#include "NarrowCollisionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/Components.h"
#include "Physics/Collision/ColliderHelper.h"
#include "Physics/Collision/PhysicsMaterial.h"

namespace Behemoth
{
	// TODO:
	// Find a way to break this function up, too large / unreadable
	void NarrowCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		// Dynamic entities are the entities that we are first iterating over to check for collisions
		for (const auto& [dynamicEntityHandle, dynamicTransformComp, collisionPairsComp] : registry.Get<TransformComponent, BroadCollisionPairsComponent>())
		{
			// Generates a tuple with all narrow colliers ** Some maybe nullptr's so always check if valid first **
			auto dynamicColliders = GetColliders(registry, dynamicEntityHandle, NarrowColliderTypes{});

			// Hit entities are all entities that collided with the dynamic entity during the broad collision check
			for (const auto& hitEntityHandle : collisionPairsComp->collisionIDs)
			{
				auto hitColliders = GetColliders(registry, hitEntityHandle, NarrowColliderTypes{});

				TransformComponent* hitTransformComp = registry.GetComponent<TransformComponent>(hitEntityHandle);
				if (!hitTransformComp)
				{
					LOGMESSAGE(Error, "Failed to get transform of colliding pair: " + registry.GetName(hitEntityHandle));
					continue;
				}

				// Essentially do a nested loop over each collider in the tuples of both the dynamic and hit entities looking for collision
				std::apply([&](auto&&... dynamicEntityColliders) 
				{
					auto CheckDynamicColliders = [&](auto&& dynamicCollider) 
					{
						return ((std::apply([&](auto&&... hitColliders)
						{
 							ContactData contactData{};
							auto CheckHitColliders = [&](auto&& collider1Comp, auto&& collider2Comp)
 							{
 								if (IsCollision(dynamicTransformComp, hitTransformComp, collider1Comp, collider2Comp, contactData))
 								{
 									contactData.physicsMaterial = collider2Comp->physicsMaterial;
 									if (collider2Comp->isTrigger)
 									{
 										GenerateData<TriggerDataComponent>(registry, dynamicEntityHandle, hitEntityHandle, contactData);
 									}
 									else
 									{
 										GenerateData<CollisionDataComponent>(registry, dynamicEntityHandle, hitEntityHandle, contactData);
 									}
 
 								}
 							}; (..., CheckHitColliders(dynamicCollider, hitColliders));

						}, hitColliders))); };

					(..., CheckDynamicColliders(dynamicEntityColliders));
				}, dynamicColliders);
			}
		}
	}
}