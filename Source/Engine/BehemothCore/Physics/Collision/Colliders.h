#pragma once
#include "Math/MathCore.h"

namespace Behemoth
{
	enum class ColliderType
	{
		None = 0,
		AABB,
		OBB,
		Sphere,
		CompoundCollider
	};

	struct Collider
	{
	};

	struct OBBCollider
	{
		OBBCollider() = default;
		BMath::Vector3 position;
		BMath::Vector3 orientation[3];
		BMath::Vector3 extents;
	};

	struct AABBCollider
	{
		AABBCollider() = default;
		BMath::Vector3 worldPosition;
		BMath::Vector3 worldExtents;
	};

	struct SphereCollider
	{
		SphereCollider() = default;
		BMath::Vector3 position;
		float radius;
	};

	// Made up of multiple OBB colliders
	struct CompoundCollider
	{
		std::vector<OBBCollider> compoundColliders;
	};
}