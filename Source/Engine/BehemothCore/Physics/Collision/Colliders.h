#pragma once
#include "Math/MathCore.h"
#include "Geometry/Plane.h"

namespace Behemoth
{
	enum class ColliderType
	{
		None = 0,
		AABB,
		Sphere,
		OBB
	};

	struct AABBCollider
	{
		AABBCollider() = default;
		BMath::Vector3 worldPosition;
		BMath::Vector3 worldExtents;
	};

	struct SphereCollider
	{
		SphereCollider() : radius(1.0f) {}
		BMath::Vector3 position;
		float radius;
	};

	struct OBBCollider
	{
		OBBCollider() = default;
		BMath::Vector3 position;
		BMath::Vector3 orientation[3];
		BMath::Vector3 extents;
	};

}