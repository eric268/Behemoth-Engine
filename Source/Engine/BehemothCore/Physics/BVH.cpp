#include "pch.h"
#include "BVH.h"
#include "Physics/Collision/Colliders.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Core/Log.h"
#include "Collision/BroadCollision.h"
#include "Misc/BColors.h"

namespace Behemoth
{
	BVHFactory::BVHFactory(bool drawDebugColliders) : root(), drawDebugColliders(drawDebugColliders) {}

	void BVHFactory::DestroyBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
	{
		for (const auto& entityHandle : entityHandles)
		{
			registry.DestroyEntity(entityHandle);
		}
		entityHandles.clear();
	}

	void BVHFactory::GenerateBVHTree(
		ECS::Registry& registry,
		std::vector<ECS::EntityHandle>& entityHandles,
		std::shared_ptr<BVHNode> nodePtr,
		std::vector<BVHData> bvhData,
		int depth)
	{
		if (!bvhData.size() || !nodePtr)
		{
			return;
		}

		// Calculate bounding box for all components and total surface area for SAH
		float bestCost = std::numeric_limits<float>::max();
		int bestAxis = -1;
		int bestPosition = -1;

		// Try splitting along each axis
		for (int axis = 0; axis < 3; axis++)
		{
			// Sort components based on their centroid along the current axis
			std::sort(bvhData.begin(), bvhData.end(), [axis](const BVHData& a, const BVHData& b) 
				{
					return a.collider.position[axis] < b.collider.position[axis];
				});

			// Apply SAH to find the best split for this axis
			for (size_t position = 1; position < bvhData.size(); ++position) 
			{
				// Calculate cost for this particular split using SAH
				float cost = CalculateSAH(position, bvhData, 1.0f, 2.0f); // Implement this based on SAH formula

				if (cost < bestCost) 
				{
					bestCost = cost;
					bestAxis = axis;
					bestPosition = position;
				}
			}
		}

		// Check if no beneficial split was found or bestAxis is still -1 indicating no split improvement
		if (bestAxis == -1) 
		{
			// Make this a leaf node, as splitting doesn't reduce cost
			return;
		}

		// Now split components into two sets based on bestAxis and bestPosition
		std::vector<BVHData> leftComponents(bvhData.begin(), bvhData.begin() + bestPosition);
		std::vector<BVHData> rightComponents(bvhData.begin() + bestPosition, bvhData.end());

		if (leftComponents.size() > 1)
		{
			nodePtr->leftChild = GenerateNode(registry, entityHandles, GenerateCollider(leftComponents), Behemoth::GetColor(Behemoth::Blue));
			GenerateBVHTree(registry, entityHandles, nodePtr->leftChild, leftComponents, depth + 1);
		}
		else if (leftComponents.size())
		{
			nodePtr->leftChild = GenerateLeaf(leftComponents[0], registry, entityHandles);
		}

		if (rightComponents.size() > 1)
		{
			nodePtr->rightChild = GenerateNode(registry, entityHandles, GenerateCollider(rightComponents), Behemoth::GetColor(Behemoth::Green));
			GenerateBVHTree(registry, entityHandles, nodePtr->rightChild, rightComponents, depth + 1);
		}
		else if (rightComponents.size())
		{
			nodePtr->rightChild = GenerateLeaf(rightComponents[0], registry, entityHandles);
		}
	}

	std::shared_ptr<BVHNode> BVHFactory::GenerateNode(
		ECS::Registry& registry,
		std::vector<ECS::EntityHandle>& entityHandles,
		const AABBCollider& collider,
		BMath::Vector3 color)
	{
		std::shared_ptr<BVHNode> nodePtr = std::make_shared<BVHNode>();
		nodePtr->collider = collider;

// Drawing the BVH colliders is just needed for debugging purposes, do not need to actually create entities in the registry for it to function
#ifdef DEBUG
		if (drawDebugColliders)
		{
			ECS::EntityHandle entityHandle = registry.CreateEntity("BVH Debug Collider");
			entityHandles.push_back(entityHandle);

			registry.AddComponent<TransformComponent>(entityHandle);
			registry.AddComponent<MoveComponent>(entityHandle, collider.position);
			registry.AddComponent<AABBColliderComponent>(entityHandle, collider.extents);
	
			registry.AddComponent<MeshInitializeComponent>(entityHandle);
			registry.AddComponent<WireframeComponent>(
				entityHandle, 
				"cube.obj",
				collider.extents,
				false,
				true,
				color);
		}
#endif
		return nodePtr;
	}

	std::shared_ptr<BVHNode> BVHFactory::GenerateLeaf(
		const BVHData& colliderData,
		ECS::Registry& registry,
		std::vector<ECS::EntityHandle>& entityHandles,
		BMath::Vector3 color)
	{
		std::shared_ptr<BVHNode> nodePtr = std::make_shared<BVHNode>();
		nodePtr->collider = colliderData.collider;
		nodePtr->entityHandle = colliderData.handle;
		nodePtr->leftChild = nullptr;
		nodePtr->rightChild = nullptr;

#ifdef DEBUG
 		if (drawDebugColliders)
 		{
 			ECS::EntityHandle entityHandle = registry.CreateEntity("BVH Debug Collider");
 			entityHandles.push_back(entityHandle);
 
 			registry.AddComponent<TransformComponent>(entityHandle);
 			registry.AddComponent<MoveComponent>(entityHandle, nodePtr->collider.position);
 			registry.AddComponent<AABBColliderComponent>(entityHandle, nodePtr->collider.extents);
 
 			auto meshInit = registry.AddComponent<MeshInitializeComponent>(entityHandle);
 			auto wireFrame = registry.AddComponent<WireframeComponent>(
				entityHandle,
				"cube.obj",
				nodePtr->collider.extents,
				false,
				true,
				color);
 		}
#endif
		return nodePtr;
	}

	float BVHFactory::GetSurfaceArea(const AABBCollider& collider)
	{
		float l = 2.0f * collider.extents.x;
		float w = 2.0f * collider.extents.y;
		float h = 2.0f * collider.extents.z;
		return 2.0f * (l * w + l * h + w * h);
	}

	float BVHFactory::CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost)
	{
		std::vector<BVHData> leftColliders(colliders.begin(), colliders.begin() + position);
		std::vector<BVHData> rightColliders(colliders.begin() + position, colliders.end());

		const int numLeft = leftColliders.size();
		const int numRight = rightColliders.size();

		AABBCollider leftBound = GenerateCollider(leftColliders);
		AABBCollider rightBound = GenerateCollider(rightColliders);

		float leftSurfaceArea = GetSurfaceArea(leftBound);
		float rightSurfaceArea = GetSurfaceArea(rightBound);

		return traversalCost + (numLeft * leftSurfaceArea + numRight * rightSurfaceArea) * intersectCost;
	}

	AABBCollider BVHFactory::GenerateCollider(const std::vector<BVHData>& colliders)
	{
		BMath::Vector3 minPos = BMath::Vector3(std::numeric_limits<float>::max());
		BMath::Vector3 maxPos = BMath::Vector3(std::numeric_limits<float>::lowest());

		for (const BVHData& data : colliders)
		{
			BMath::Vector3 colliderMin = data.collider.position - data.collider.extents;
			BMath::Vector3 colliderMax = data.collider.position + data.collider.extents;

			for (int i = 0; i < 3; i++)
			{
				if (colliderMin[i] < minPos[i])
				{
					minPos[i] = colliderMin[i];
				}

				if (colliderMax[i] > maxPos[i])
				{
					maxPos[i] = colliderMax[i];
				}
			}
		}

		AABBCollider collider{};
		collider.position = (maxPos + minPos) / 2.0f;
		collider.extents = (maxPos - minPos) / 2.0f;
		return collider;
	}
}
