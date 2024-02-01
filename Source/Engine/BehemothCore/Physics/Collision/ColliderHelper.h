#pragma once

#include "Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"
#include "ECS/Entity.h"
#include "ECS/Registry.h"

namespace Behemoth
{
	// TODO:
	// Find a way to remove all nullptr colliders so we don't need to waste cycles iterating over them later
	template <typename ...Colliders>
	static std::tuple<Colliders*...> GetColliders(ECS::Registry& registry, const ECS::EntityHandle& handle, NarrowColliders<Colliders ...>)
	{
		return registry.GetMultipleComponents<Colliders...>(handle);
	}

	template<typename T>
	inline void SetCollider(const TransformComponent* transformComp, T* colliderComponent);

	template<>
	inline void SetCollider(const TransformComponent* transformComp, BVHColliderComponent* colliderComp)
	{
		if (!transformComp || !colliderComp)
		{
			return;
		}

		colliderComp->collider.extents = transformComp->worldScale * colliderComp->extents;
		colliderComp->collider.position = transformComp->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transformComp, AABBColliderComponent* colliderComp)
	{
		if (!transformComp || !colliderComp)
		{
			return;
		}

		colliderComp->collider.extents  = transformComp->worldScale * colliderComp->extents;
		colliderComp->collider.position = transformComp->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transformComp, SphereColliderComponent* colliderComp)
	{
		if (!transformComp || !colliderComp)
		{
			return;
		}

		colliderComp->collider.center = transformComp->worldPosition;

		// Since there is the potential for non-uniform scaling, and ellipse collider is not supported always scale with max of three axis scaling
		colliderComp->collider.radius = std::max(transformComp->worldScale[0], 
							      std::max(transformComp->worldScale[1], transformComp->worldScale[2])) * colliderComp->radius;
	}

	template<>
	inline void SetCollider(const TransformComponent* transformComp, OBBColliderComponent* colliderComp)
	{
		if (!transformComp || !colliderComp)
		{
			return;
		}
		colliderComp->collider.position = transformComp->worldPosition;
		colliderComp->collider.extents = transformComp->worldScale * colliderComp->extents;

		BMath::Matrix3x3 orientationMatrix = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform, transformComp->worldScale);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				colliderComp->collider.orientation[i][j] = orientationMatrix.data[i][j];
			}
		}
	}
}