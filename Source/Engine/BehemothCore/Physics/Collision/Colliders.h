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
		Mesh
	};

	struct Collider
	{
	};

	struct OBBCollider
	{
		OBBCollider() = default;
		OBBCollider(BMath::Vector3 extent) : halfwidthExtents(extent) {}
		BMath::Vector3 pos;
		BMath::Vector3 orientation[3];
		BMath::Vector3 halfwidthExtents;
	};

	struct AABBCollider
	{
		AABBCollider() = default;
		AABBCollider(BMath::Vector3 extents) : extents(extents) {}
		BMath::Vector3 position;
		BMath::Vector3 extents;
	};

	struct SphereCollider
	{
		SphereCollider() = default;
		SphereCollider(float radius) : radius(radius) {}
		BMath::Vector3 positionOffset;
		float radius;
	};

	struct MeshCollider
	{
		BMath::Vector3 pos;
	};
}