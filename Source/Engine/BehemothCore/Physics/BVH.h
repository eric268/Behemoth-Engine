#pragma once

#include "Physics/Colliders.h"
#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include <vector>
#include <tuple>
#include <memory>

namespace ECS
{
	// class Registry;
}

namespace Behemoth
{
	class TransformComponent;
	class AABBColliderComponent;
}

namespace Behemoth::Collision
{
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

		void OnConstruction(ECS::Registry& registry);
		void OnReconstruction(ECS::Registry& registry);

	private:
		std::vector<BVHData> GetColliderData(ECS::Registry& registry);
		std::shared_ptr<BVHNode> CreateRootNode(const std::vector<BVHData>& data);
		AABBCollider GenerateBound(const std::vector<BVHData>& colliders);

		void GenerateBVHTree(ECS::Registry& DEBUG_registry, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth);



		float GetSurfaceArea(const AABBCollider& componenets);
		float CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost);

		std::shared_ptr<BVHNode> root;

		std::vector<ECS::EntityHandle> DEBUG_ColliderHandles;

		// Debug
		ECS::EntityHandle rootDebugEntity;
		void GenerateDebugBound(ECS::Registry& DEBUG_registry, AABBCollider collider, Math::Vector3 color);

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

