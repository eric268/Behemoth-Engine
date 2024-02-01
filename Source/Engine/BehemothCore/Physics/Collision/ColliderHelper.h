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
	inline void SetCollider(const TransformComponent* transform, T* colliderComponent);

	template<>
	inline void SetCollider(const TransformComponent* transform, BVHColliderComponent* colliderComp)
	{
		if (!transform || !colliderComp)
		{
			return;
		}

		colliderComp->collider.extents = transform->worldScale * colliderComp->extents;
		colliderComp->collider.position = transform->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, AABBColliderComponent* colliderComp)
	{
		if (!transform || !colliderComp)
		{
			return;
		}

		colliderComp->collider.extents = transform->worldScale * colliderComp->extents;
		colliderComp->collider.position = transform->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, SphereColliderComponent* colliderComp)
	{
		if (!transform || !colliderComp)
		{
			return;
		}

		colliderComp->collider.center = transform->worldPosition;

		// Since there is the potential for non-uniform scaling, and ellipse collider is not supported always scale with max of three axis scaling
		colliderComp->collider.radius = std::max(transform->worldScale[0], 
							  std::max(transform->worldScale[1], transform->worldScale[2])) * colliderComp->radius;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, OBBColliderComponent* colliderComp)
	{
		if (!transform || !colliderComp)
		{
			return;
		}
		colliderComp->collider.position = transform->worldPosition;
		colliderComp->collider.extents = transform->worldScale * colliderComp->extents;

		BMath::Matrix3x3 orientationMatrix = TransformHelper::ExtractRotationMatrix(transform->worldTransform, transform->worldScale);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				colliderComp->collider.orientation[i][j] = orientationMatrix.data[i][j];
			}
		}
	}
}