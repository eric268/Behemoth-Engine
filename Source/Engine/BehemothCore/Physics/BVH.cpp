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
		std::vector<BVHData> data;
		root = CreateRootNode(registry, data);

		rootDebugEntity = registry.CreateEntity();
		registry.AddComponent<TransformComponent>(rootDebugEntity);
		registry.AddComponent<MoveComponent>(rootDebugEntity, root->debugColliderPos);
		registry.AddComponent<WireframeComponent>(rootDebugEntity, "cube.obj", root->debugColliderScale, true, Math::Vector3(1.0f, 0.0f, 0.0f));
		registry.AddComponent<MeshInitalizeComponent>(rootDebugEntity);
		registry.AddComponent<AABBColliderComponent>(rootDebugEntity, root->debugColliderScale);

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
		root = CreateRootNode(registry, data);

		rootDebugEntity = registry.CreateEntity();
		registry.AddComponent<TransformComponent>(rootDebugEntity);
		registry.AddComponent<MoveComponent>(rootDebugEntity, root->debugColliderPos);
		registry.AddComponent<WireframeComponent>(rootDebugEntity, "cube.obj", root->debugColliderScale, true, Math::Vector3(1.0f, 0.0f, 0.0f));
		registry.AddComponent<MeshInitalizeComponent>(rootDebugEntity);
		registry.AddComponent<AABBColliderComponent>(rootDebugEntity, root->debugColliderScale);

		GenerateBVHTree(registry, root, data, 1);
	}

	std::shared_ptr<BVHNode> BVH::CreateRootNode(ECS::Registry& registry, std::vector<BVHData>& data)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();

		std::shared_ptr<BVHNode> rootNode= std::make_shared<BVHNode>();

		Math::Vector3 minPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 maxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		data.clear();
		data.reserve(components.size());

		for (const auto& [entity, transformComp, colliderComp] : components)
		{
			// Potentially don't need to pass transform as well, just the collider
			colliderComp->collider.pos = transformComp->position;
			Math::Vector3 colliderMin = colliderComp->collider.pos - colliderComp->collider.halfwidthExtents;
			Math::Vector3 colliderMax = colliderComp->collider.pos + colliderComp->collider.halfwidthExtents;

			// Add data for the rest of tree creation
			data.push_back({ entity, colliderComp->collider });

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

		rootNode->collider.pos = (maxPos + minPos) / 2.0f;
		rootNode->collider.halfwidthExtents = (maxPos - minPos) / 2.0f;
		rootNode->leftChild = nullptr;
		rootNode->rightChild = nullptr;

#ifdef DEBUG
		Math::Vector3 debugMinPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 debugMaxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		for (const auto& [entity, transformComp, colliderComp] : components)
		{
			Math::Vector3 debugColliderMin = transformComp->position - colliderComp->collider.halfwidthExtents * 2.0f;
			Math::Vector3 debugColliderMax = transformComp->position + colliderComp->collider.halfwidthExtents * 2.0f;

			for (int i = 0; i < 3; i++)
			{
				if (debugColliderMin[i] < debugMinPos[i])
				{
					debugMinPos[i] = debugColliderMin[i];
				}

				if (debugColliderMax[i] > debugMaxPos[i])
				{
					debugMaxPos[i] = debugColliderMax[i];
				}
			}
		}
		rootNode->debugColliderPos = (debugMaxPos + debugMinPos) / 2.0f;
		rootNode->debugColliderScale = (debugMaxPos - debugMinPos) / 2.0f;
#endif
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

		
		AABBCollider DEBUG_leftDebugBound = GenerateBound(leftComponents, true);
		AABBCollider DEBUG_rightDebugBound = GenerateBound(rightComponents, true);

		// Left 
		GenerateDebugBound(DEBUG_registry, DEBUG_leftDebugBound, colors[depth]);
		// Right
		GenerateDebugBound(DEBUG_registry, DEBUG_rightDebugBound, colors[depth]);

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

	AABBCollider BVH::GenerateBound(std::vector<BVHData>& colliders, bool debug)
	{
		Math::Vector3 minPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 maxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		for (const BVHData& data : colliders)
		{
			float multiplier = (debug) ? 2.0f : 1.0f;

			Math::Vector3 colliderMin = data.collider.pos - data.collider.halfwidthExtents * multiplier;
			Math::Vector3 colliderMax = data.collider.pos + data.collider.halfwidthExtents * multiplier;

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

	void BVH::GenerateDebugBound(ECS::Registry& DEBUG_registry, AABBCollider& debugBound, Math::Vector3 color)
	{
		ECS::EntityHandle handle = DEBUG_registry.CreateEntity();
		DEBUG_registry.AddComponent<TransformComponent>(handle);
		DEBUG_registry.AddComponent<MoveComponent>(handle, debugBound.pos);
		DEBUG_registry.AddComponent<WireframeComponent>(handle, "cube.obj", debugBound.halfwidthExtents, true, color);
		DEBUG_registry.AddComponent<MeshInitalizeComponent>(handle);

		DEBUG_ColliderHandles.push_back(handle);
	}
}
