#include "pch.h"
#include "NarrowCollisionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/Components.h"
#include "Physics/Collision/ColliderHelper.h"

namespace Behemoth
{
	void NarrowCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [dynamicEntity, dynamicTransform, collisionPairs] : registry.Get<TransformComponent, BroadCollisionPairsComponent>())
		{
			auto dynamicColliders = GetColliders(registry, dynamicEntity, AllColliderComponents{});

			for (const auto& hitEntityID : collisionPairs->nodeIDs)
			{
				auto hitColliders = GetColliders(registry, hitEntityID, AllColliderComponents{});

				TransformComponent* hitTransform = registry.GetComponent<TransformComponent>(hitEntityID);

				if (!hitTransform)
				{
					LOGMESSAGE(Error, "Failed to get transform of colliding pair: " + registry.GetName(hitEntityID));
					continue;
				}

				std::apply([&](auto&&... elems1) 
				{
						auto func = [&](auto&& tup) 
						{
							return ((std::apply([&](auto&&... elems2)
							{
								ContactData contactData{};
								auto OnCollision = [&](auto&& entitiy, auto&& hitEntity)
								{
									if (GenerateCollisionData(dynamicTransform, hitTransform, entitiy, hitEntity, contactData))
									{
										CollisionDataComponent* collisionDataComp = registry.GetComponent<CollisionDataComponent>(dynamicEntity);
										if (!collisionDataComp)
										{
											collisionDataComp = registry.AddComponent<CollisionDataComponent>(dynamicEntity);
										}

										if (collisionDataComp)
										{
											BMath::Vector3 hitVelocity{};

											if (VelocityComponent* hitVelocityComponent = registry.GetComponent<VelocityComponent>(hitEntityID))
											{
												hitVelocity = hitVelocityComponent->velocity;
											}
											CollisionData collisionData(contactData, hitEntityID, hitVelocity);
											collisionDataComp->data.push_back(collisionData);
										}
									}
								}; (..., OnCollision(tup, elems2));

							}, hitColliders))); };

						(..., func(elems1));
				}, dynamicColliders);

			}
		}
	}
}