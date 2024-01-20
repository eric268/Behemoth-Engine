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
	inline void SetCollider(const TransformComponent* transform, BVHColliderComponent* component)
	{
		if (!transform || !component)
		{
			return;
		}

		component->collider.worldExtents = transform->worldScale * component->extents;
		component->collider.worldPosition = transform->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, AABBColliderComponent* component)
	{
		if (!transform || !component)
		{
			return;
		}

		component->collider.worldExtents = transform->worldScale * component->extents;
		component->collider.worldPosition = transform->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, SphereColliderComponent* component)
	{
		if (!transform || !component)
		{
			return;
		}

		component->collider.position = transform->worldPosition;
		component->collider.radius = std::max(transform->worldScale[0], std::max(transform->worldScale[1], transform->worldScale[2])) * component->radius;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, OBBColliderComponent* component)
	{
		if (!transform || !component)
		{
			return;
		}
		component->collider.position = transform->worldPosition;
		component->collider.extents = transform->worldScale * component->extents;

		BMath::BMatrix3x3 orientationMatrix = TransformHelper::ExtractRotationMatrix(transform->worldTransform);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				component->collider.orientation[i][j] = orientationMatrix.data[i][j];
			}
		}
	}
}