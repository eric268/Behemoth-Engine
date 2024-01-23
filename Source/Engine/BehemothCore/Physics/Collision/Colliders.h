#pragma once
#include "Math/MathCore.h"
#include "Geometry/Plane.h"
#include "Components/Components.h"
#include "Misc/TransformHelper.h"

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
		BMath::Vector3 position;
		BMath::Vector3 extents;
	};

	struct SphereCollider
	{
		SphereCollider() : radius(1.0f) {}
		BMath::Vector3 center;
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