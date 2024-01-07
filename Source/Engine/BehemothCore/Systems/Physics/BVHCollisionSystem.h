#pragma once
#include "Math/Vector.h"
#include "ECS/Entity.h"

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
	class TransformComponent;
	class VelocityComponent;
	class RigidBodyComponent;
	class AABBColliderComponent;
	class BVHNode;

	class BVHCollisionSystem
	{
	public:
		using DynamicEntities = std::vector <std::tuple<ECS::Entity, RigidBodyComponent*, VelocityComponent*, TransformComponent*, AABBColliderComponent*>>;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void CheckStaticCollision(ECS::Registry& registry, DynamicEntities& entities);
		void CheckDynamicCollision(ECS::Registry& registry, DynamicEntities& entities);

		bool CheckLineCollision(ECS::EntityHandle handle, Math::Vector3 p1, Math::Vector3 p2, std::shared_ptr<BVHNode> root);
		bool CheckAABBCollision(ECS::EntityHandle handle, const AABBCollider& collider, std::shared_ptr<BVHNode> root);
	};
}
