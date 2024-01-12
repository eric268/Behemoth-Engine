#pragma once
#include "Math/MathCore.h"
#include "Components/PhysicsComponents.h"

namespace Behemoth
{
	using real = double;

	class OBBCollider;
	class SphereCollider;
	class CollisionData;
	class ContactData;
	class Plane;

	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData);
	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData);

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, CollisionData& collisionData);
	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& collisionData);

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

}

