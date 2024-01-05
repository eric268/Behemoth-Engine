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

		// Only for drawing the debug wireframe collider
		Math::Vector3 debugColliderPos;
		Math::Vector3 debugColliderScale;
		
		// For leaf nodes only
		ECS::EntityHandle entityHandles;
	};

	struct BVHData
	{
		ECS::EntityHandle handle;
		AABBCollider collider;
		BVHData() = default;
		BVHData(ECS::EntityHandle handle, AABBCollider collider) : handle(handle), collider(collider) {}
	};

	class BVH
	{
	public:
		BVH();
		~BVH();

		void OnConstruction(ECS::Registry& registry);
		void OnReconstruction(ECS::Registry& registry);

	private:
		std::shared_ptr<BVHNode> CreateRootNode(ECS::Registry& registry, std::vector<BVHData>& data);

		void GenerateBVHTree(ECS::Registry& DEBUG_registry, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth);

		AABBCollider GenerateBound(std::vector<BVHData>& colliders, bool debug = false);

		float GetSurfaceArea(const AABBCollider& componenets);
		float CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost);

		std::shared_ptr<BVHNode> root;

		std::vector<ECS::EntityHandle> DEBUG_ColliderHandles;

		// Debug
		ECS::EntityHandle rootDebugEntity;
		void GenerateDebugBound(ECS::Registry& DEBUG_registry, AABBCollider& debugBound, Math::Vector3 color = Math::Vector3(1.0f, 0.0f, 0.0f));

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

