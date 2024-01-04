#pragma once

#include "Physics/Colliders.h"

#include <vector>
#include <tuple>
#include <memory>

namespace ECS
{
	class Registry;
	class Entity;
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
		BVHNode* leftChild;
		BVHNode* rightChild;
		Math::Vector3 debugColliderPos;
		Math::Vector3 debugColliderScale;
		ECS::Entity* entity;
	};

	class BVH
	{
	public:
		BVH();
		~BVH();

		void OnConstruction(ECS::Registry& registry);
		void OnReconstruction(ECS::Registry& registry);

	private:
		std::shared_ptr<BVHNode> CreateRootNode(std::vector<std::tuple<ECS::Entity, TransformComponent*, AABBColliderComponent*>> components);
		std::shared_ptr<BVHNode> root;

		float GetSurfaceArea(const AABBCollider& componenets);
		float CalculateSAH(float traversalCost, std::vector<AABBCollider>& leftColliders, std::vector<AABBCollider>& rightColliders, float intersectCost);
	};
}

