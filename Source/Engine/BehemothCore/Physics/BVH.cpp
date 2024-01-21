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
	BVHFactory::BVHFactory() : root(), drawDebugColliders(false) {}
	BVHFactory::~BVHFactory() {}

	void BVHFactory::DestroyBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles)
	{
		for (const auto& handle : entityHandles)
		{
			registry.DestroyEntity(handle);
		}
		entityHandles.clear();
	}

	void BVHFactory::GenerateBVHTree(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth)
	{
		if (!data.size() || !node)
		{
			return;
		}

		// Calculate bounding box for all components and total surface area for SAH
		float bestCost = std::numeric_limits<float>::max();
		int bestAxis = -1;
		int bestPosition = -1;

		// Try splitting along each axis (x, y, z)
		for (int axis = 0; axis < 3; axis++)
		{
			// Sort components based on their centroid along the current axis
			std::sort(data.begin(), data.end(), [axis](const BVHData& a, const BVHData& b) 
				{
					return a.collider.worldPosition[axis] < b.collider.worldPosition[axis];
				});

			// Apply SAH to find the best split for this axis
			for (size_t position = 1; position < data.size(); ++position) 
			{
				// Calculate cost for this particular split using SAH
				float cost = CalculateSAH(position, data, 1.0f, 2.0f); // Implement this based on SAH formula

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
		std::vector<BVHData> leftComponents(data.begin(), data.begin() + bestPosition);
		std::vector<BVHData> rightComponents(data.begin() + bestPosition, data.end());

		// Assign children to current node
		if (leftComponents.size() > 1)
		{
			node->leftChild = GenerateNode(registry, entityHandles, GenerateCollider(leftComponents), Behemoth::GetColor(Behemoth::Blue));
			GenerateBVHTree(registry, entityHandles, node->leftChild, leftComponents, depth + 1);
		}
		else if (leftComponents.size())
		{
			node->leftChild = GenerateLeaf(leftComponents[0]);
		}

		if (rightComponents.size() > 1)
		{
			node->rightChild = GenerateNode(registry, entityHandles, GenerateCollider(rightComponents), Behemoth::GetColor(Behemoth::Green));
			GenerateBVHTree(registry, entityHandles, node->rightChild, rightComponents, depth + 1);
		}
		else if (rightComponents.size())
		{
			node->rightChild = GenerateLeaf(rightComponents[0]);
		}
	}


	float BVHFactory::GetSurfaceArea(const AABBCollider& aabb)
	{
		float l = 2.0f * aabb.worldExtents.x;
		float w = 2.0f * aabb.worldExtents.y;
		float h = 2.0f * aabb.worldExtents.z;
		return 2.0f * (l * w + l * h + w * h);
	}

	float BVHFactory::CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost)
	{
		std::vector<BVHData> leftColliders(colliders.begin(), colliders.begin() + position);
		std::vector<BVHData> rightColliders(colliders.begin() + position, colliders.end());

		const int numLeft =  leftColliders.size();
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
			BMath::Vector3 colliderMin = data.collider.worldPosition - data.collider.worldExtents * data.entityScale;
			BMath::Vector3 colliderMax = data.collider.worldPosition + data.collider.worldExtents * data.entityScale;

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
		collider.worldPosition = (maxPos + minPos) / 2.0f;
		collider.worldExtents = (maxPos - minPos) / 2.0f;
		return collider;
	}

	std::shared_ptr<BVHNode> BVHFactory::GenerateNode(ECS::Registry& registry, std::vector<ECS::EntityHandle>& entityHandles, const AABBCollider& collider, BMath::Vector3 color)
	{
		std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
		node->collider = collider;

// Drawing the BVH colliders is just needed for debugging purposes, do not need to actually create entities in the registry for it to function
#ifdef DEBUG
		if (drawDebugColliders)
		{
			ECS::EntityHandle handle = registry.CreateEntity("BVH Debug Collider");
			entityHandles.push_back(handle);

			registry.AddComponent<TransformComponent>(handle);
			registry.AddComponent<MoveComponent>(handle, collider.worldPosition);
			registry.AddComponent<AABBColliderComponent>(handle, collider.worldExtents);
	
			registry.AddComponent<MeshInitalizeComponent>(handle);
			registry.AddComponent<WireframeComponent>(handle, "cube.obj", collider.worldExtents,false, true, color);
		}
#endif

		return node;
	}

	std::shared_ptr<BVHNode> BVHFactory::GenerateLeaf(const BVHData& colliderData)
	{
		std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
		node->collider = colliderData.collider;
		node->collider.worldExtents *= colliderData.entityScale;
		node->entityHandle = colliderData.handle;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
}
