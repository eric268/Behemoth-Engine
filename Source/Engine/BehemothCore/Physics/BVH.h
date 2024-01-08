#pragma once

#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Misc/BColors.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"

#include <vector>
#include <tuple>
#include <memory>

namespace Behemoth
{
	class AABBCollider;

	struct BVHNode
	{
		BVHNode() : entityHandles(NULL_ENTITY) {}
		bool IsLeaf() 
		{ 
			return entityHandles.ID != NULL_ENTITY; 
		}

		std::string name;
		AABBCollider collider;
		std::shared_ptr<BVHNode> leftChild;
		std::shared_ptr<BVHNode> rightChild;
		// For leaf nodes only
		ECS::EntityHandle entityHandles;
	};

	struct BVHData
	{
		BVHData() : handle(), collider() {}
		BVHData(ECS::EntityHandle handle, AABBCollider collider) : handle(handle), collider(collider) {}
		ECS::EntityHandle handle;
		AABBCollider collider;
	};

	class BVHFactory
	{
	public:
		BVHFactory();
		~BVHFactory();

		template <typename ...T>
		std::shared_ptr<BVHNode> OnConstruction(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
		{
			return CreateBVHTree<T...>(registry, entityHandles);
		}

		template<typename ...T>
		std::shared_ptr<BVHNode> OnReconstruction(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
		{
			DestroyBVHTree(registry);
			return CreateBVHTree<T...>(registry, entityHandles);
		}

	private:

		template <typename ...T>
		std::shared_ptr<BVHNode> CreateBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
		{
			std::vector<BVHData> data = GetSceneColliderData<T...>(registry);
			root = GenerateNode(registry, entityHandles, GenerateCollider(data), true, BColors::GetColor(BColors::Red));
			GenerateBVHTree(registry, entityHandles, root, data, 1);
			return root;
		}

		template <typename ...T>
		std::vector<BVHData> GetSceneColliderData(ECS::Registry& registry)
		{
			auto components = registry.Get<TransformComponent, AABBColliderComponent, T...>();
			std::vector<BVHData> data;
			data.reserve(components.size());

			for (auto& componentTuple : components)
			{
				std::apply([&data](ECS::Entity entity, TransformComponent* transformComp, AABBColliderComponent* colliderComp, auto* ...args)
					{

						colliderComp->collider.position = transformComp->position;
						data.push_back(BVHData(entity, colliderComp->collider));

					}, componentTuple);
 			}
			return data;
		}


		AABBCollider GenerateCollider(const std::vector<BVHData>& colliders);
		std::shared_ptr<BVHNode> GenerateNode(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, const AABBCollider& collider, bool drawCollider = true, BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));
		std::shared_ptr<BVHNode> GenerateLeaf(const BVHData& colliderData);

		void GenerateBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth);
		float CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost);
		float GetSurfaceArea(const AABBCollider& componenets);

		
		void DestroyBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles);

		std::shared_ptr<BVHNode> root;

		int DEBUG_ColliderID = 0;
	};
}

