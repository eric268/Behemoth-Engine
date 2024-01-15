#pragma once
#include "Math/MathCore.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth
{
	using real = double;

	struct OBBCollider;
	struct SphereCollider;
	struct CollisionData;
	struct ContactData;
	class Plane;

	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData);
	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData);

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, std::vector<ContactData>& data);
	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& collisionData);
	bool NarrowSphereOBBCollision(const SphereCollider& sphere, const OBBCollider& box, ContactData& collisionData);

	bool NarrowOBBOBBCollision(const OBBCollider box1, const OBBCollider box2, ContactData& contactData);

	void SetSATBestPen(int& bestIndex, real& bestPen, real absDistance, real combinedBoxes, int index);
	void OBBVertexFaceCollision(const OBBCollider& box1, const OBBCollider& box2, const BMath::Vector3& toCenter, ContactData& contactData, int bestIndex, real pen);

	BMath::Vector3 CalculateOBBContactPoint(
			const BMath::Vector3& pointOneEdge,
			const BMath::Vector3& oneAxis,
			real oneAxisExtent,
			const BMath::Vector3& pointTwoEdge,
			const BMath::Vector3& twoAxis,
			real twoAxisExtent,
			const bool twoSize);

	bool TryAxis(
		const OBBCollider& box1,
		const OBBCollider& box2, 
		BMath::Vector3 axis, 
		const BMath::Vector3 toCenter, 
		int index, 
		real& smallestPen,
		int& smallestCase);

	real penetrationOnAxis(
		const OBBCollider& box1,
		const OBBCollider& box2,
		BMath::Vector3 axis,
		const BMath::Vector3& toCenter);

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
		return NarrowSphereOBBCollision(c1, c2, contactData);
	}

	template<>
	inline bool CheckCollision(const OBBCollider& c1, const OBBCollider& c2, ContactData& contactData)
	{
		return NarrowOBBOBBCollision(c1, c2, contactData);
	}




// 	bool DEBUG_boxAndBox(
// 		const OBBCollider& one,
// 		const OBBCollider& two,
// 		ContactData data);

}

