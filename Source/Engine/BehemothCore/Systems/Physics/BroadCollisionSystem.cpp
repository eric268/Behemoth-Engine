#include "pch.h"
#include "BroadCollisionSystem.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"
#include "Physics/Collision/BroadCollision.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Physics/BVH.h"


namespace Behemoth
{
	void BroadCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		registry.RemoveAllComopnents<BroadCollisionPairsComponent>();
		registry.RemoveAllComopnents<CollisionDataComponent>();
		registry.RemoveAllComopnents<TriggerDataComponent>();

		DynamicEntities  dynamicEntities = registry.Get<RigidBodyComponent, VelocityComponent, TransformComponent, BVHColliderComponent>();
		CheckCollision<StaticComponent>(registry, dynamicEntities, deltaTime);
		CheckCollision<RigidBodyComponent>(registry, dynamicEntities, deltaTime);
	}

	bool BroadCollisionSystem::CheckAABBCollision(
		ECS::EntityHandle entityHandle,
		const AABBCollider& collider,
		std::shared_ptr<BVHNode> root,
		std::vector<ECS::EntityHandle>& collidingHandles)
	{
		using node = std::shared_ptr<Behemoth::BVHNode>;

		std::stack<node> nodes;
		nodes.push(root);

		while (nodes.size())
		{
			node currentNode = nodes.top();
			nodes.pop();

			if (!currentNode || entityHandle == currentNode->entityHandle)
			{
				continue;
			}

			if (Behemoth::BroadAABBCollision(collider, currentNode->collider))
			{
				if (currentNode->IsLeaf())
				{
					collidingHandles.push_back(currentNode->entityHandle);
				}
				else
				{
					nodes.push(currentNode->leftChild);
					nodes.push(currentNode->rightChild);
				}
			}
		}
		return collidingHandles.size();
	}
}