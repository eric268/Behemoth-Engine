#include "pch.h"
#include "BVHCollisionSystem.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"
#include "Physics/Collision/BroadCollision.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Physics/BVH.h"


namespace Behemoth
{
	void BVHCollisionSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		DynamicEntities  dynamicEntities = registry.Get<RigidBodyComponent, VelocityComponent, TransformComponent, AABBColliderComponent>();
		CheckStaticCollision(registry, dynamicEntities);
		// CheckDynamicCollision(registry, dynamicEntities);
	}

	void BVHCollisionSystem::CheckStaticCollision(ECS::Registry& registry, DynamicEntities&  dynamicEntities)
	{
		auto staticBVHComponents = registry.Get<BVHComponent<StaticComponent>>();

		for (const auto& [entity, rigidBody, Velocity, transform, collider] : dynamicEntities)
		{
			// There should only be one static BVH
			for (const auto& [entity, bvhRoot] : staticBVHComponents)
			{
				collider->collider.position = transform->position;
				if (CheckAABBCollision(entity, collider->collider, bvhRoot->rootNode))
				{
					std::cout << "Static Collision\n";
				}

				// Do a seperate check for other colliders
			}
		}
	}

	void BVHCollisionSystem::CheckDynamicCollision(ECS::Registry& registry, DynamicEntities&  dynamicEntities)
	{
		auto dynamicBVHComponent = registry.Get<BVHComponent<RigidBodyComponent>>();


		for (const auto& [entity, rigidBody, Velocity, transform, collider] : dynamicEntities)
		{
			// There should only be one dynamic BVH
			for (const auto& [entity, bvhRoot] : dynamicBVHComponent)
			{
				collider->collider.position = transform->position;
				if (CheckAABBCollision(entity, collider->collider, bvhRoot->rootNode))
				{
					std::cout << "Dynamic Collision\n";
				}

				// Do a sperate check for other colliders
			}
		}
	}

	bool BVHCollisionSystem::CheckLineCollision(ECS::EntityHandle handle, Math::Vector3 p1, Math::Vector3 p2, std::shared_ptr<BVHNode> root)
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

	bool BVHCollisionSystem::CheckAABBCollision(ECS::EntityHandle handle, const AABBCollider& collider, std::shared_ptr<BVHNode> root)
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

			if (handle.ID != currentNode->entityHandles.ID && Behemoth::BroadAABBCollision(collider, currentNode->collider))
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
}