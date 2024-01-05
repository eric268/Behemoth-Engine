#include "pch.h"
#include "BVH.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth::Collision
{
	BVH::BVH() : root()
	{

	}

	BVH::~BVH()
	{

	}

	void BVH::OnConstruction(ECS::Registry& registry)
	{
		std::vector<BVHData> data = GetColliderData(registry);
		root = CreateRootNode(data);

  		rootDebugEntity = registry.CreateEntity("BVH Root Bound");
		registry.AddComponent<TransformComponent>(rootDebugEntity);
		registry.AddComponent<MoveComponent>(rootDebugEntity, root->collider.pos);
		registry.AddComponent<WireframeComponent>(rootDebugEntity, "cube.obj", root->collider.halfwidthExtents, true, Math::Vector3(1.0f, 0.0f, 0.0f));
		registry.AddComponent<MeshInitalizeComponent>(rootDebugEntity);
 		registry.AddComponent<AABBColliderComponent>(rootDebugEntity, root->collider.halfwidthExtents);

		 GenerateBVHTree(registry, root, data, 1);
	}

	void BVH::OnReconstruction(ECS::Registry& registry)
	{
		for (int i = 0; i < DEBUG_ColliderHandles.size(); i++)
		{
			registry.DestroyEntity(DEBUG_ColliderHandles[i]);
		}

		registry.DestroyEntity(rootDebugEntity);

		std::vector<BVHData> data;
		root = CreateRootNode(data);

		rootDebugEntity = registry.CreateEntity();
		registry.AddComponent<TransformComponent>(rootDebugEntity);
		registry.AddComponent<MoveComponent>(rootDebugEntity, root->collider.pos);
		registry.AddComponent<WireframeComponent>(rootDebugEntity, "cube.obj", root->collider.halfwidthExtents, true, Math::Vector3(1.0f, 0.0f, 0.0f));
		registry.AddComponent<MeshInitalizeComponent>(rootDebugEntity);
		registry.AddComponent<AABBColliderComponent>(rootDebugEntity, root->collider.halfwidthExtents);

		// GenerateBVHTree(registry, root, data, 1);
	}

	std::vector<BVHData> BVH::GetColliderData(ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();
		std::vector<BVHData> data;
		data.reserve(components.size());

		for (const auto& [entity, transformComp, colliderComp] : components)
		{
			colliderComp->collider.pos = transformComp->position;
			data.push_back({ entity, colliderComp->collider });
		}
		return data;
	}

	std::shared_ptr<BVHNode> BVH::CreateRootNode(const std::vector<BVHData>& data)
	{
		std::shared_ptr<BVHNode> rootNode= std::make_shared<BVHNode>();

		AABBCollider rootBound = GenerateBound(data);

		rootNode->collider = rootBound;
		
		return rootNode;
	}

	void BVH::GenerateBVHTree(ECS::Registry& DEBUG_registry, std::shared_ptr<BVHNode> node, std::vector<BVHData> data, int depth)
	{
		// Base case: If the components are few, make this a leaf node
		if (data.size() == 1 )
		{
			node->leftChild = std::make_shared<BVHNode>();
			node->leftChild->entityHandles = data[0].handle;
			node->leftChild->collider = data[0].collider;
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
					return a.collider.pos[axis] < b.collider.pos[axis];
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

		// Recursively build left and right subtrees
		std::shared_ptr<BVHNode> leftChild = std::make_shared<BVHNode>();
		std::shared_ptr<BVHNode> rightChild = std::make_shared<BVHNode>();

		// Assign children to current node
		node->leftChild = leftChild;
		node->rightChild = rightChild;
		
		leftChild->collider		= GenerateBound(leftComponents);
		rightChild->collider	= GenerateBound(rightComponents);

		// Left 
		GenerateDebugBound(DEBUG_registry, leftChild->collider, colors[depth]);
		// Right
		GenerateDebugBound(DEBUG_registry, rightChild->collider, colors[depth]);

		GenerateBVHTree(DEBUG_registry, leftChild, leftComponents, depth + 1);
		GenerateBVHTree(DEBUG_registry, rightChild, rightComponents, depth + 1);
	}


	float BVH::GetSurfaceArea(const AABBCollider& aabb)
	{
		float l = 2.0f * aabb.halfwidthExtents.x;
		float w = 2.0f * aabb.halfwidthExtents.y;
		float h = 2.0f * aabb.halfwidthExtents.z;
		return 2.0f * (l * w + l * h + w * h);
	}

	float BVH::CalculateSAH(int position, std::vector<BVHData>& colliders, float traversalCost, float intersectCost)
	{
		std::vector<BVHData> leftColliders(colliders.begin(), colliders.begin() + position);
		std::vector<BVHData> rightColliders(colliders.begin() + position, colliders.end());

		const int numLeft =  leftColliders.size();
		const int numRight = rightColliders.size();

		AABBCollider leftBound = GenerateBound(leftColliders);
		AABBCollider rightBound = GenerateBound(rightColliders);

		float leftSurfaceArea = GetSurfaceArea(leftBound);
		float rightSurfaceArea = GetSurfaceArea(rightBound);

		return traversalCost + (numLeft * leftSurfaceArea + numRight * rightSurfaceArea) * intersectCost;
	}

	AABBCollider BVH::GenerateBound(const std::vector<BVHData>& colliders)
	{
		Math::Vector3 minPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 maxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		for (const BVHData& data : colliders)
		{
			Math::Vector3 colliderMin = data.collider.pos - data.collider.halfwidthExtents;
			Math::Vector3 colliderMax = data.collider.pos + data.collider.halfwidthExtents;

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
		collider.pos = (maxPos + minPos) / 2.0f;
		collider.halfwidthExtents = (maxPos - minPos) / 2.0f;
		return collider;
	}

	void BVH::GenerateDebugBound(ECS::Registry& DEBUG_registry, AABBCollider collider, Math::Vector3 color)
	{
		ECS::EntityHandle handle = DEBUG_registry.CreateEntity("BVH immediate leaf ");
		DEBUG_registry.AddComponent<TransformComponent>(handle);
		DEBUG_registry.AddComponent<MoveComponent>(handle, collider.pos);
		DEBUG_registry.AddComponent<WireframeComponent>(handle, "cube.obj", collider.halfwidthExtents, true, color);
		DEBUG_registry.AddComponent<MeshInitalizeComponent>(handle);
		DEBUG_registry.AddComponent<AABBColliderComponent>(handle, collider.halfwidthExtents);

		DEBUG_ColliderHandles.push_back(handle);
	}
}
