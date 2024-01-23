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
	struct AABBCollider;

	struct BVHNode
	{
		BVHNode() : entityHandle(NULL_ENTITY) {}
		bool IsLeaf() 
		{ 
			return entityHandle.ID != NULL_ENTITY; 
		}

		std::string name;
		AABBCollider collider;
		std::shared_ptr<BVHNode> leftChild;
		std::shared_ptr<BVHNode> rightChild;
		// For leaf nodes only
		ECS::EntityHandle entityHandle;
	};

	struct BVHData
	{
		BVHData() : handle(), collider() {}
		BVHData(ECS::EntityHandle handle, AABBCollider collider, BMath::Vector3 scale) : handle(handle), collider(collider), entityScale(scale) {}
		ECS::EntityHandle handle;
		AABBCollider collider;
		BMath::Vector3 entityScale;
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
			DestroyBVHTree(registry, entityHandles);
			return CreateBVHTree<T...>(registry, entityHandles);
		}

		bool drawDebugColliders;
	
	private:

		template <typename ...T>
		std::shared_ptr<BVHNode> CreateBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
		{
			std::vector<BVHData> data = GetSceneColliderData<T...>(registry);

			if (!data.size())
			{
				return nullptr;
			}

			// If only one entity exists then we only need a single lead node for the entire tree
			if (data.size() == 1)
			{
				root = GenerateLeaf(data[0], registry, entityHandles);
			}
			else
			{
				root = GenerateNode(registry, entityHandles, GenerateCollider(data), Behemoth::GetColor(Behemoth::Red));
			}

			GenerateBVHTree(registry, entityHandles, root, data, 1);
			return root;
		}

		template <typename ...T>
		std::vector<BVHData> GetSceneColliderData(ECS::Registry& registry)
		{
			auto components = registry.Get<TransformComponent, BVHColliderComponent, T...>();
			std::vector<BVHData> data;
			data.reserve(components.size());

			for (auto& componentTuple : components)
			{
				std::apply([&data](ECS::Entity entity, TransformComponent* transformComp, BVHColliderComponent* colliderComp, auto* ...args)
					{

						colliderComp->collider.position = transformComp->worldPosition;
						data.push_back(BVHData(entity, colliderComp->collider, transformComp->worldScale));

					}, componentTuple);
 			}
			return data;
		}


		AABBCollider GenerateCollider(const std::vector<BVHData>& colliders);
		std::shared_ptr<BVHNode> GenerateNode(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, const AABBCollider& collider, BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));
		std::shared_ptr<BVHNode> GenerateLeaf(const BVHData& colliderData, ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));

		void GenerateBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth);
		float CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost);
		float GetSurfaceArea(const AABBCollider& componenets);

		void DestroyBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles);
		std::shared_ptr<BVHNode> root;
	};
}

