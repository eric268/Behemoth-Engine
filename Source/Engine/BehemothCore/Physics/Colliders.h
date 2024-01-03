#pragma once
#include "Math/MathCore.h"

namespace Behemoth::Collision
{
	enum ColliderType
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

	struct OBBCollider : public Collider
	{
		OBBCollider() = default;
		OBBCollider(Math::Vector3 extent) : halfwidthExtents(extent) {}
		Math::Vector3 pos;
		Math::Vector3 orientation[3];
		Math::Vector3 halfwidthExtents;
	};

	struct AABBCollider : public Collider
	{
		AABBCollider() = default;
		AABBCollider(Math::Vector3 extents) : halfwidthExtents(extents) {}
		Math::Vector3 pos;
		Math::Vector3 halfwidthExtents;
	};

	struct SphereCollider : public Collider
	{
		SphereCollider() = default;
		SphereCollider(float radius) : radius(radius) {}
		Math::Vector3 pos;
		float radius;
	};

	struct MeshCollider : public Collider
	{
		Math::Vector3 pos;
	};

	template <int T>
	struct GetColliderType;

	template<>
	struct GetColliderType<AABB>
	{
		using Type = AABBCollider;
	};

	template<>
	struct GetColliderType<OBB>
	{
		using Type = OBBCollider;
	};

	template<>
	struct GetColliderType<Sphere>
	{
		using Type = SphereCollider;
	};

	template <>
	struct GetColliderType<Mesh>
	{
		using Type = MeshCollider;	
	};
}