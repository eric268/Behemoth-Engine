#pragma once
#include "Math/MathCore.h"
#include "Components/PhysicsComponents.h"
#include "Physics/Ray.h"

namespace Behemoth
{
	using real = double;

	struct OBBCollider;
	struct SphereCollider;
	struct CollisionData;
	struct ContactData;
	class Plane;

	bool NarrowRayOBBCollision(const Ray& ray, const OBBCollider& box, ContactData& contactData);
	bool NarrowRaySphereCollision(const Ray& ray, const SphereCollider& sphere, ContactData& contactData);

	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData);
	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData);

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, std::vector<ContactData>& data);
	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& collisionData);
	bool NarrowSphereOBBCollision(const SphereCollider& sphere, const OBBCollider& box, ContactData& collisionData);

	bool NarrowOBBOBBCollision(const OBBCollider box1, const OBBCollider box2, ContactData& contactData);


	template<typename T, typename U>
	bool CheckCollision(const T&, const U&, ContactData& contactData);

	template<>
	inline bool CheckCollision(const SphereCollider& c1, const SphereCollider& c2, ContactData& contactData)
	{
		return NarrowSphereSphereCollision(c1, c2, contactData);
	}

	template<>
	inline bool CheckCollision(const OBBCollider& c1, const SphereCollider& c2, ContactData& contactData)
	{
		return NarrowOBBSphereCollision(c1, c2, contactData);
	}

	template<>
	inline bool CheckCollision(const SphereCollider& c1, const OBBCollider& c2, ContactData& contactData)
	{
		bool result = NarrowSphereOBBCollision(c1, c2, contactData);
		if (result)
			NarrowSphereOBBCollision(c1, c2, contactData);
		return result;
	}

	template<>
	inline bool CheckCollision(const OBBCollider& c1, const OBBCollider& c2, ContactData& contactData)
	{
		// return NarrowOBBOBBCollision(c1, c2, contactData);
		bool result = NarrowOBBOBBCollision(c1, c2, contactData);
		if (result)
			NarrowOBBOBBCollision(c1, c2, contactData);
		return result;
	}

	template<>
	inline bool CheckCollision(const Ray& ray, const SphereCollider& sphere, ContactData& contactData)
	{
		return  NarrowRaySphereCollision(ray, sphere, contactData);
	}

	template<>
	inline bool CheckCollision(const Ray& ray, const OBBCollider& box, ContactData& contactData)
	{
		return NarrowRayOBBCollision(ray, box, contactData);
	}
}

