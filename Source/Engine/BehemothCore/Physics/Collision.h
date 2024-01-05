#pragma once
#include "Components/PhysicsComponents.h"

namespace Behemoth::Collision
{
	bool CheckAABBCollision(const AABBCollider& box1, const AABBCollider& box2);
	bool CheckSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2);
	bool CheckOBBCollision(const OBBCollider& box1, const OBBCollider& box2);
	bool CheckSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box);

	template <typename T, typename U>
	bool CheckCollision(const T& a, const U& b);

	template<>
	inline bool CheckCollision<AABBCollider, AABBCollider>(const AABBCollider& a, const AABBCollider& b)
	{
		return CheckAABBCollision(a, b);
	}

	template<>
	inline bool CheckCollision<SphereCollider, SphereCollider>(const SphereCollider& s1, const SphereCollider& s2)
	{
		return CheckSphereSphereCollision(s1, s2);
	}

	template<>
	inline bool CheckCollision<OBBCollider, OBBCollider>(const OBBCollider& c1, const OBBCollider& c2)
	{
		return CheckOBBCollision(c1, c2);
	}

	template<>
	inline bool CheckCollision<SphereCollider, AABBCollider>(const SphereCollider& sphere, const AABBCollider& box)
	{
		return CheckSphereAABBCollision(sphere, box);
	}
}