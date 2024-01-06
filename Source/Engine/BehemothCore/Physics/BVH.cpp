#include "pch.h"
#include "BVH.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Misc/Log.h"

namespace Behemoth::Collision
{
	BVH::BVH() : root(), treeType(BVHTreeType::None) {}

	BVH::~BVH() {}

	void BVH::OnConstruction(ECS::Registry& registry, BVHTreeType type)
	{
		treeType = type;
		CreateBVHTree(registry);
	}

	void BVH::OnReconstruction(ECS::Registry& registry)
	{
		DestroyBVHTree(registry);
		CreateBVHTree(registry);
	}

	void BVH::CreateBVHTree(ECS::Registry& registry)
	{
		std::vector<BVHData> data = GetSceneColliderData(registry);
		root = GenerateNode(registry, GenerateCollider(data), true, colors[0]);
		GenerateBVHTree(registry, root, data, 1);
	}

	void BVH::DestroyBVHTree(ECS::Registry& registry)
	{
		for (const auto& handle : colliderHandles)
		{
			registry.DestroyEntity(handle);
		}
		colliderHandles.clear();
	}

	std::vector<BVHData> BVH::GetSceneColliderData(ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();
		std::vector<BVHData> data;
		data.reserve(components.size());

		for (const auto& [entity, transformComp, colliderComp] : components)
		{
			colliderComp->collider.position = transformComp->position;
			data.push_back(BVHData(entity, colliderComp->collider));
		}
		return data;
	}

	void BVH::GenerateBVHTree(ECS::Registry& registry, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth)
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
					return a.collider.position[axis] < b.collider.position[axis];
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
			node->leftChild = GenerateNode(registry, GenerateCollider(leftComponents), true, colors[depth]);
			GenerateBVHTree(registry, node->leftChild, leftComponents, depth + 1);
		}
		else if (leftComponents.size())
		{
			node->leftChild = GenerateLeaf(leftComponents[0]);
		}

		if (rightComponents.size() > 1)
		{
			node->rightChild = GenerateNode(registry, GenerateCollider(rightComponents), true, colors[depth]);
			GenerateBVHTree(registry, node->rightChild, rightComponents, depth + 1);
		}
		else if (rightComponents.size())
		{
			node->rightChild = GenerateLeaf(rightComponents[0]);
		}
	}


	float BVH::GetSurfaceArea(const AABBCollider& aabb)
	{
		float l = 2.0f * aabb.extents.x;
		float w = 2.0f * aabb.extents.y;
		float h = 2.0f * aabb.extents.z;
		return 2.0f * (l * w + l * h + w * h);
	}

	float BVH::CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost)
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

	AABBCollider BVH::GenerateCollider(const std::vector<BVHData>& colliders)
	{
		Math::Vector3 minPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 maxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		for (const BVHData& data : colliders)
		{
			Math::Vector3 colliderMin = data.collider.position - data.collider.extents;
			Math::Vector3 colliderMax = data.collider.position + data.collider.extents;

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

	std::shared_ptr<BVHNode> BVH::GenerateNode(ECS::Registry& registry, AABBCollider collider, bool drawCollider, Math::Vector3 color)
	{
		std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
		node->collider = collider;

		ECS::EntityHandle handle = registry.CreateEntity("BVH Collider");
		colliderHandles.push_back(handle);

		registry.AddComponent<TransformComponent>(handle);
		registry.AddComponent<MoveComponent>(handle, collider.position);
		registry.AddComponent<AABBColliderComponent>(handle, collider.extents);
		
		if (drawCollider)
		{
			registry.AddComponent<WireframeComponent>(handle, "cube.obj", collider.extents, true, color);
			registry.AddComponent<MeshInitalizeComponent>(handle);
		}

		return node;
	}

	std::shared_ptr<BVHNode> BVH::GenerateLeaf(const BVHData& colliderData)
	{
		std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
		node->collider = colliderData.collider;
		node->entityHandles = colliderData.handle;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
}
