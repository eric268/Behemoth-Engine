#pragma once
#include "Math/Vector.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Physics/BVH.h"
#include "Physics/Collision/ColliderHelper.h"

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

			for (const auto& [
							dynamicEntityHandle, 
					type, 
					velocity,
					transform,
					colliderComp] : dynamicEntities)
			{
				SetCollider(transform, colliderComp);

				for (const auto& [entities, bvhRootComp] : bvhComponent)
				{
					std::vector<ECS::EntityHandle> hitEntityHandles;

					if (CheckAABBCollision(dynamicEntityHandle, colliderComp->collider, bvhRootComp->rootNode, hitEntityHandles))
					{
						// Check if entity already has a collision pairs component
						if (BroadCollisionPairsComponent* collisionPairsComp = registry.GetComponent<BroadCollisionPairsComponent>(dynamicEntityHandle))
						{
							// If it already has the component add the new collision pairs to the container
							std::vector<ECS::EntityHandle>& collisionPairs = collisionPairsComp->collisionIDs;
							collisionPairs.insert(collisionPairs.end(), hitEntityHandles.begin(), hitEntityHandles.end());
						}
						else
						{
							// Otherwise add component
							registry.AddComponent<BroadCollisionPairsComponent>(dynamicEntityHandle, hitEntityHandles);
						}
					}
 				}
			}
		}

		bool CheckAABBCollision(
			ECS::EntityHandle entityHandle,
			const AABBCollider& collider,
			std::shared_ptr<BVHNode> root,
			std::vector<ECS::EntityHandle>& nodeHandles);
	};
}
