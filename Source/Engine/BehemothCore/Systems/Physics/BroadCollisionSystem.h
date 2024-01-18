#pragma once
#include "Math/Vector.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/BVH.h"

#include <vector>
#include <tuple>
#include <memory>

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct AABBCollider;
	struct BVHNode;

	class BroadCollisionSystem
	{
	public:
		using DynamicEntities = std::vector<std::tuple<ECS::Entity, RigidBodyComponent*, VelocityComponent*, TransformComponent*, BVHColliderComponent*>>;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		template<typename T>
		void CheckCollision(ECS::Registry& registry, DynamicEntities& dynamicEntities, const float deltaTime)
		{
			auto bvhComponent = registry.Get<BVHRootComponent<T>>();

			for (const auto& [dynamicEntity, type, velocity, transform, collider] : dynamicEntities)
			{
				
				collider->collider.worldExtents = transform->worldScale * collider->extents;
				collider->collider.worldPosition = transform->worldPosition;

				for (const auto& [entities, bvhRootComp] : bvhComponent)
				{
					if (dynamicEntity == entities)
					{
						continue;
					}

					std::vector<ECS::EntityHandle> nodeHandles;
 					// This is for AABB do a separate one for lines or other collider types
					if (CheckAABBCollision(dynamicEntity, collider->collider, bvhRootComp->rootNode, nodeHandles))
					{
						// Check if entity already has a collision pairs component
						BroadCollisionPairsComponent* collisionPairsComp = registry.GetComponent<BroadCollisionPairsComponent>(dynamicEntity);
						if (!collisionPairsComp)
						{
							registry.AddComponent<BroadCollisionPairsComponent>(dynamicEntity, nodeHandles);
						}
						else
						{
							// If it already has the component add the new collision pairs to the end of that container
							std::vector<ECS::EntityHandle>& collisionPairs = collisionPairsComp->collisionIDs;
							collisionPairs.insert(collisionPairs.end(), nodeHandles.begin(), nodeHandles.end());
						}
					}
 				}
			}
		}

		bool CheckAABBCollision(ECS::EntityHandle handle, const AABBCollider& collider, std::shared_ptr<BVHNode> root, std::vector<ECS::EntityHandle>& nodeHandles);
	};
}
