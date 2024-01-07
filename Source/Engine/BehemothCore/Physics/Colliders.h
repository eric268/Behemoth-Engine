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
		OBBCollider(Math::Vector3 extent) : halfwidthExtents(extent) {}
		Math::Vector3 pos;
		Math::Vector3 orientation[3];
		Math::Vector3 halfwidthExtents;
	};

	struct AABBCollider
	{
		AABBCollider() = default;
		AABBCollider(Math::Vector3 extents) : extents(extents) {}
		Math::Vector3 position;
		Math::Vector3 extents;
	};

	struct SphereCollider
	{
		SphereCollider() = default;
		SphereCollider(float radius) : radius(radius) {}
		Math::Vector3 position;
		float radius;
	};

	struct MeshCollider
	{
		Math::Vector3 pos;
	};
}