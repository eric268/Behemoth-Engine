#include "pch.h"
#include "Ray.h"
#include "ECS/Registry.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"
#include "Physics/BVH.h"
#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/NarrowCollision.h"
#include "Physics/Collision/ColliderHelper.h"

namespace Behemoth
{
	Ray::Ray() {}
	Ray::Ray(BMath::Vector3 position, BMath::Vector3 direction) : origin(position), direction(direction) {}



	bool BroadRayCheck(ECS::Registry& registry, const Ray& ray, std::vector<ECS::EntityHandle>& hitEntities, const std::vector<ECS::EntityHandle>& entitiesToIgnore);
	bool NarrowRayCheck(ECS::Registry& registry, const Ray& ray, const std::vector<ECS::EntityHandle>& hitEntities, std::vector<ContactData>& data, BMask::CollisionType mask);
	
	template<typename T>
	bool RayBVHCheck(ECS::Registry& registry, const Ray& ray, const std::vector<ECS::EntityHandle>& entitiesToIgnore, std::vector<ECS::EntityHandle>& entitiesHit);

	bool RayCast(ECS::Registry& registry, const Ray& ray, std::vector<ContactData>& data, const std::vector<ECS::EntityHandle>& entitiesToIgnore, BMask::CollisionType mask)
	{
		std::vector<ECS::EntityHandle> hitEntities;

		if (!BroadRayCheck(registry, ray, hitEntities, entitiesToIgnore))
		{
			return false;
		}

		std::vector<ContactData> hitData;
		return NarrowRayCheck(registry, ray, hitEntities, hitData, mask);

		return false;
	}

	bool RayCast(ECS::Registry& registry, const Ray& ray, ContactData& data, const std::vector<ECS::EntityHandle>& entitiesToIgnore, BMask::CollisionType mask)
	{
		std::vector<ECS::EntityHandle> hitEntities;

		if (!BroadRayCheck(registry, ray, hitEntities, entitiesToIgnore))
		{
			return false;
		}

		std::vector<ContactData> hitData;
		return NarrowRayCheck(registry, ray, hitEntities, hitData, mask);

		return false;

	}

	static bool BroadRayCheck(ECS::Registry& registry, const Ray& ray, std::vector<ECS::EntityHandle>& hitEntities, const std::vector<ECS::EntityHandle>& entitiesToIgnore)
	{
		// Static Entities
		RayBVHCheck<StaticComponent>(registry, ray, entitiesToIgnore, hitEntities);
		RayBVHCheck<RigidBodyComponent>(registry, ray, entitiesToIgnore, hitEntities);
		return hitEntities.size();
	}

	static bool NarrowRayCheck(ECS::Registry& registry, const Ray& ray, const std::vector<ECS::EntityHandle>& hitEntities, std::vector<ContactData>& data, BMask::CollisionType mask)
	{
		for (const auto& entity : hitEntities)
		{
			auto colliders = GetColliders(registry, entity, AllColliderComponents{});

			std::apply([&](auto&& ... collider)
				{
					auto NarrrowRayCheck = [&](const Ray& r, auto&& c)
					{
						ContactData contactData{};
						{
							// Ensure collider isn't null, that mask is correct and that collision is occurring 
							if (c && c->collisionType & mask && CheckCollision(r, c, contactData))
							{
								data.push_back(contactData);
							}
						}

					}; (..., (NarrrowRayCheck(ray, collider)));
				},
				colliders);
		}

		return data.size();
	}

	static bool CheckRayCollision(const Ray& ray, const std::shared_ptr<BVHNode> root, const std::vector<ECS::EntityHandle>& entitiesToIgnore, std::vector<ECS::EntityHandle>& hitEntity)
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

			bool ignoreEntity = false;
			for (int i = 0; i < entitiesToIgnore.size(); i++)
			{
				if (currentNode->entityHandles.ID == entitiesToIgnore[i].ID)
				{
					ignoreEntity = true;
					break;
				}
			}

			if (!ignoreEntity && Behemoth::BroadRayAABBIntersection(ray, currentNode->collider))
			{
				if (currentNode->IsLeaf())
				{
					hitEntity.push_back(currentNode->entityHandles.ID);
					continue;
				}

				nodes.push(currentNode->leftChild);
				nodes.push(currentNode->rightChild);
			}
		}
		return hitEntity.size();
	}

	template<typename T>
	static bool RayBVHCheck(ECS::Registry& registry, const Ray& ray, const std::vector<ECS::EntityHandle>& entitiesToIgnore, std::vector<ECS::EntityHandle>& entitiesHit)
	{
		auto bvhComponent = registry.Get<BVHComponent<T>>();
		// Should only be one BVH node with static or dynamic type, but leave check in for now
		for (const auto& bvhNode : bvhComponent)
		{
			const auto& [handle, node] = bvhNode;
			CheckRayCollision(ray, node->rootNode, entitiesToIgnore, entitiesHit);
		}
		return entitiesHit.size();
	}
}