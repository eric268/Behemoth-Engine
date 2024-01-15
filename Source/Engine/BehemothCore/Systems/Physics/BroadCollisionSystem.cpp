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

		DynamicEntities  dynamicEntities = registry.Get<RigidBodyComponent, VelocityComponent, TransformComponent, BroadColliderComponent>();
		CheckCollision<StaticComponent>(registry, dynamicEntities, deltaTime);
		// CheckCollision<RigidBodyComponent>(registry, dynamicEntities, deltaTime);
	}

	bool BroadCollisionSystem::CheckLineCollision(ECS::EntityHandle handle, BMath::Vector3 p1, BMath::Vector3 p2, std::shared_ptr<BVHNode> root)
	{
		using node = std::shared_ptr<Behemoth::BVHNode>;

		std::stack<node> nodes;
		nodes.push(root);
		bool collisionFound = false;

		while (nodes.size())
		{
			node currentNode = nodes.top();
			nodes.pop();

			if (!currentNode)
			{
				continue;
			}

			if (handle.ID != currentNode->entityHandles.ID && Behemoth::BroadLineAABBIntersection(p1, p2, currentNode->collider))
			{
				if (currentNode->IsLeaf())
				{
					std::cout << "Collision with entity handle: " << currentNode->entityHandles.ID << std::endl;
					collisionFound = true;
					continue;
				}
// 				else
// 				{
// 					std::cout << "Collision with BVH volume named: " << currentNode->name << std::endl;
// 				}

				nodes.push(currentNode->leftChild);
				nodes.push(currentNode->rightChild);
			}
		}
		return collisionFound;
	}

	bool BroadCollisionSystem::CheckAABBCollision(ECS::EntityHandle handle, const AABBCollider& collider, std::shared_ptr<BVHNode> root, std::vector<ECS::EntityHandle>& nodeHandles)
	{
		using node = std::shared_ptr<Behemoth::BVHNode>;

		std::stack<node> nodes;
		nodes.push(root);

		while (nodes.size())
		{
			node currentNode = nodes.top();
			nodes.pop();

			if (!currentNode)
			{
				continue;
			}
			if (Behemoth::BroadAABBCollision(collider, currentNode->collider))
			{
				if (currentNode->IsLeaf())
				{
					nodeHandles.push_back(currentNode->entityHandles);
					continue;
				}
				nodes.push(currentNode->leftChild);
				nodes.push(currentNode->rightChild);
			}
		}
		return nodeHandles.size();
	}
}