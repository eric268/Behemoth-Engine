#pragma once

#include "Physics/Colliders.h"
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
	class TransformComponent;
	class AABBColliderComponent;
}

namespace Behemoth::Collision
{
	enum class BVHTreeType
	{
		None,
		Static,
		Dynamic
	};

	struct BVHNode
	{
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

	class BVH
	{
	public:
		BVH();
		~BVH();

		void OnConstruction(ECS::Registry& registry, BVHTreeType type);
		void OnReconstruction(ECS::Registry& registry);
		inline std::shared_ptr<BVHNode> GetRoot() { return root; }

	private:
		std::vector<BVHData> GetSceneColliderData(ECS::Registry& registry);
		AABBCollider GenerateCollider(const std::vector<BVHData>& colliders);
		std::shared_ptr<BVHNode> GenerateNode(ECS::Registry& registry, AABBCollider collider, bool drawCollider = true, Math::Vector3 color = Math::Vector3(1.0f, 1.0f, 1.0f));
		std::shared_ptr<BVHNode> GenerateLeaf(const BVHData& colliderData);

		void GenerateBVHTree(ECS::Registry& registry, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth);
		float CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost);
		float GetSurfaceArea(const AABBCollider& componenets);

		void CreateBVHTree(ECS::Registry& registry);
		void DestroyBVHTree(ECS::Registry& registry);

		BVHTreeType treeType;
		std::shared_ptr<BVHNode> root;
		std::vector<ECS::EntityHandle> colliderHandles;

		Math::Vector3 colors[7] =
		{
			Math::Vector3(1.0f, 0.0f, 0.0f),
			Math::Vector3(0.0f, 1.0f, 0.0f),
			Math::Vector3(0.0f, 0.0f, 1.0f),
			Math::Vector3(1.0f, 0.0f, 1.0f),
			Math::Vector3(1.0f, 1.0f, 0.0f),
			Math::Vector3(0.0f, 1.0f, 1.0f),
			Math::Vector3(1.0f, 1.0f, 1.0f)
		};
	};
}

