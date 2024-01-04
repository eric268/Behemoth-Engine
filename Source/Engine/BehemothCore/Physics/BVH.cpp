#include "pch.h"
#include "BVH.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth::Collision
{
	BVH::BVH() : root(nullptr)
	{

	}

	BVH::~BVH()
	{

	}

	void BVH::OnConstruction(ECS::Registry& registry)
	{
		auto components = registry.Get<TransformComponent, AABBColliderComponent>();
		root = CreateRootNode(components);

		ECS::Entity rootDebugEntity = registry.CreateEntity();
		registry.AddComponent<TransformComponent>(rootDebugEntity);
		registry.AddComponent<MoveComponent>(rootDebugEntity, root->collider.pos);
		registry.AddComponent<WireframeComponent>(rootDebugEntity, "cube.obj", root->collider.halfwidthExtents + Math::Vector3(0.5f), true, Math::Vector3(1.0f, 0.0f, 0.0f));
		registry.AddComponent<MeshInitalizeComponent>(rootDebugEntity);
	}

	void BVH::OnReconstruction(ECS::Registry& registry)
	{

	}

	std::shared_ptr<BVHNode> BVH::CreateRootNode(std::vector<std::tuple<ECS::Entity, TransformComponent*, AABBColliderComponent*>> components)
	{
		std::shared_ptr<BVHNode> rootNode = std::make_shared<BVHNode>();

		Math::Vector3 minPos = Math::Vector3(std::numeric_limits<float>::max());
		Math::Vector3 maxPos = Math::Vector3(std::numeric_limits<float>::lowest());

		for (const auto& [entity, transformComp, colliderComp] : components)
		{
			Math::Vector3 colliderMin = transformComp->position - colliderComp->collider.halfwidthExtents;
			Math::Vector3 colliderMax = transformComp->position + colliderComp->collider.halfwidthExtents;

			for (int i = 0; i < 3; i++)
			{
				if (colliderMin[i] < minPos[i])
					minPos[i] = colliderMin[i];

				if (colliderMax[i] > maxPos[i])
					maxPos[i] = colliderMax[i];
			}
		}
		rootNode->collider.pos = (maxPos + minPos) / 2.0f;
		rootNode->collider.halfwidthExtents = (maxPos - minPos) / 2.0f;
		rootNode->leftChild = nullptr;
		rootNode->rightChild = nullptr;
		rootNode->entity = nullptr;

		return rootNode;
	}
}
