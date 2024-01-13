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
	class AABBCollider;
	class BVHNode;

	class BroadCollisionSystem
	{
	public:
		using DynamicEntities = std::vector <std::tuple<ECS::Entity, RigidBodyComponent*, VelocityComponent*, TransformComponent*, BroadColliderComponent*>>;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		template<typename T>
		void CheckCollision(ECS::Registry& registry, DynamicEntities& dynamicEntities)
		{
			auto bvhComponent = registry.Get<BVHComponent<StaticComponent>>();

			for (const auto& [entity, type, velocity, transform, collider] : dynamicEntities)
			{
 				 collider->collider.worldPosition = transform->worldPosition + collider->extents;

				for (const auto& [entities, bvhRootComp] : bvhComponent)
				{
					// This is for AABB do a separate one for lines or other collider types
					if (CheckAABBCollision(entity, collider->collider, bvhRootComp->rootNode))
					{
						std::cout << "BVH Collision\n";
					}
 				}
			}
		}

		bool CheckLineCollision(ECS::EntityHandle handle, BMath::Vector3 p1, BMath::Vector3 p2, std::shared_ptr<BVHNode> root);
		bool CheckAABBCollision(ECS::EntityHandle handle, const AABBCollider& collider, std::shared_ptr<BVHNode> root);
	};
}
