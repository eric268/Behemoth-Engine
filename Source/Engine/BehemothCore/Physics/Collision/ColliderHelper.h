#pragma once

#include "Colliders.h"
#include "Components/PhysicsComponents.h"
#include "Components/Components.h"

namespace Behemoth
{

	template<typename T, typename U>
	inline void SetCollider(const TransformComponent* transform, T* colliderComponent, U& collider);

	template<>
	inline void SetCollider(const TransformComponent* transform, AABBColliderComponent* component, AABBCollider& collider)
	{
		if (!transform || !component)
		{
			return;
		}

		collider.worldExtents = transform->worldScale * component->extents;
		collider.worldPosition = transform->worldPosition;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, SphereColliderComponent* component, SphereCollider& collider)
	{
		if (!transform || !component)
		{
			return;
		}

		collider.position = transform->worldPosition;
		collider.radius = std::max(transform->worldScale[0], std::max(transform->worldScale[1], transform->worldScale[2])) * component->radius;
	}

	template<>
	inline void SetCollider(const TransformComponent* transform, OBBColliderComponent* component, OBBCollider& collider)
	{
		if (!transform || !component)
		{
			return;
		}

		collider.position = transform->worldPosition;
		collider.extents = transform->worldScale * component->extents;

		BMath::Matrix3x3 orientationMatrix = TransformHelper::ExtractRotationMatrix(transform->worldTransform);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				collider.orientation[i][j] = orientationMatrix.data[i][j];
			}
		}
	}
}