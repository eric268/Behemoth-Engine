#include "pch.h"
#include "NarrowCollisionSystem.h"
#include "ECS/Registry.h"
#include "Physics/Collision/Colliders.h"
#include "Components/Components.h"

namespace Behemoth
{
	void NarrowCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, BroadCollisionPairsComponent>();

		for (const auto& [entity, TransformComponent, collisionPairs] : components)
		{
			auto dynamicColliders = GetColliders(registry, entity, AllColliderComponents{});

			for (const auto& p : collisionPairs->nodeIDs)
			{
				auto result = GetColliders(registry, p, AllColliderComponents{});
				

				ContactData data{};

				std::apply([&](auto&&... elems1) 
					{
					(std::apply([&](auto&&... elems2) 
						{

						ContactData data{};

						auto OnCollision = [&](auto&& e1, auto&& e2)
						{
							if (GenerateCollisionData(e1, e2, data))
							{
								CollisionDataComponent* collisionDataComp = registry.GetComponent<CollisionDataComponent>(entity);
								
								if (!collisionDataComp)
								{
									collisionDataComp = registry.AddComponent<CollisionDataComponent>(entity);
								}
								
								if (collisionDataComp)
								{
									BMath::Vector3 otherVelocity{};

									if (VelocityComponent* vel = registry.GetComponent<VelocityComponent>(p))
									{
										otherVelocity = vel->velocity;
									}
									CollisionData d(data, p, otherVelocity);
									collisionDataComp->data.push_back(d);
								}
							}
						};
						(..., OnCollision(elems1, elems2));

						}, result));
					}, (dynamicColliders));
			}
		}

		auto componentsToRemove = registry.GetComponent<BroadCollisionPairsComponent>();
		if (componentsToRemove)
		{
			for (int i = componentsToRemove->dense.size() - 1; i >= 0; i--)
			{
				componentsToRemove->RemoveComponent(componentsToRemove->dense[i]);
			}
		}
	}
}