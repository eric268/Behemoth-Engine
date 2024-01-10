#pragma once
#include "Math/MathCore.h"


namespace Behemoth
{
	class OBBCollider;
	class SphereCollider;
	class CollisionData;
	class ContactData;
	class Plane;

	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData);
	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData);

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, CollisionData& collisionData);
	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& collisionData);

	bool NarrowOBBOBBCollision(const OBBCollider& box1, const OBBCollider& box2, ContactData& contactData);


	void SetSATBestPen(int& bestIndex, float& bestPen, float absDistance, float combinedBoxes, int index);
	void OBBVertexFaceCollision(const OBBCollider& box1, const OBBCollider& box2, const BMath::Vector3& toCenter, ContactData& contactData, int bestIndex, float pen);
}

