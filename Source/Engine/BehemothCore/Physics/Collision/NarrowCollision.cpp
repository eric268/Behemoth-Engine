#include "pch.h"
#include "NarrowCollision.h"
#include "BroadCollision.h"
#include "Math/Matrix.h"
#include "Physics/Collision/Colliders.h"
#include "Geometry/Plane.h"
#include "CollisionData.h"

namespace Behemoth
{
	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData)
	{
		BMath::Vector3 positionOne = sphere1.position;
		BMath::Vector3 positionTwo = sphere2.position;

		BMath::Vector3 midline = positionOne - positionTwo;
		float size = BMath::Vector3::Magnitude(midline);

		if (size <= 0.0f || size > sphere1.radius + sphere2.radius)
		{
			return false;
		}

		BMath::Vector3 normal = midline / size;
		contactData.collisionNormal = normal;
		contactData.collisionPoint = positionOne + midline * 0.5f;
		contactData.penetrationDepth = (sphere1.radius + sphere2.radius - size);
		return true;
	}

	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData)
	{
		BMath::Vector3 spherePosition = sphere.position;
		float distanceFromPlane = BMath::Vector3::Dot(plane.normal, spherePosition) - sphere.radius - Plane::CalculatePlaneOffset(plane.normal, Plane::GetPointOnPlane(plane));
	
		if (distanceFromPlane >= 0.0f)
		{
			return false;
		}

		contactData.collisionNormal = plane.normal;
		contactData.penetrationDepth = -distanceFromPlane;
		contactData.collisionPoint = spherePosition - plane.normal * (distanceFromPlane + sphere.radius);
		return true;
	}

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, std::vector<ContactData>& data)
	{
		if (!BroadOBBPlaneCollision(box, plane))
		{
			return false;
		}

		static float mults[8][3] = 
		{ 
			{1,1,1},  {-1,1,1},  {1,-1,1},   {-1,-1, 1},
			{1,1,-1}, {-1,1,-1}, {1,-1,-1},  {-1,-1,-1} 
		};

		const float planeOffset = Plane::CalculatePlaneOffset(plane.normal, Plane::GetPointOnPlane(plane));

		bool collisionOccured = false;

		for (int i = 0; i < 8; i++)
		{
			BMath::Vector3 vertexPosition = BMath::Vector3(mults[i][0] * box.extents.x, mults[i][1] * box.extents.y, mults[i][2] * box.extents.z);
			
			vertexPosition = BMath::Vector3
			(
				box.orientation[0].x * vertexPosition.x + box.orientation[1].x * vertexPosition.y + box.orientation[2].x * vertexPosition.z,
				box.orientation[0].y * vertexPosition.x + box.orientation[1].y * vertexPosition.y + box.orientation[2].y * vertexPosition.z,
				box.orientation[0].z * vertexPosition.x + box.orientation[1].z * vertexPosition.y + box.orientation[2].z * vertexPosition.z
			);

			vertexPosition += box.position;

			float vertexDistance = BMath::Vector3::Dot(vertexPosition, plane.normal);

			// Vertex is penetrating plane
			if (vertexDistance <= planeOffset)
			{
				collisionOccured = true;
				ContactData contactData{};
				contactData.collisionNormal = plane.normal;
				contactData.penetrationDepth = planeOffset - vertexDistance;
				contactData.collisionPoint = vertexPosition - plane.normal * (vertexDistance - planeOffset) * 0.5f;

				// Decide later if we want to save all contacts or break out early 
				data.push_back(contactData);
			}
		}

		return collisionOccured;
	}

	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& contactData)
	{
		BMath::Vector3 spherePosition = sphere.position;
		BMath::Vector3 boxPosition = box.position;

		BMath::Vector3 translatedCenter = spherePosition - boxPosition;

		// Transpose the rotation matrix formed by the orientation vectors
		BMath::Vector3 invRotatedCenter = BMath::Vector3(
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.orientation[0].x,box.orientation[1].x, box.orientation[2].x)),
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.orientation[0].y,box.orientation[1].y, box.orientation[2].y)),
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.orientation[0].z,box.orientation[1].z, box.orientation[2].z))
		);

		BMath::Vector3 relCenter = BMath::Vector3
		(
			invRotatedCenter.x / box.extents.x,
			invRotatedCenter.y / box.extents.y,
			invRotatedCenter.z / box.extents.z
		);

		if (std::abs(relCenter.x) - sphere.radius > box.extents.x ||
			std::abs(relCenter.y) - sphere.radius > box.extents.y ||
			std::abs(relCenter.z) - sphere.radius > box.extents.z)
		{
			return false;
		}

		BMath::Vector3 closestPoint(0.0f);

		float distance = relCenter.x;
		distance = (distance > box.extents.x) ? box.extents.x : (distance < -box.extents.x) ? -box.extents.x : relCenter.x;
		closestPoint.x = distance;
	
		distance = relCenter.y;
		distance = (distance > box.extents.y) ? box.extents.y : (distance < -box.extents.y) ? -box.extents.y : relCenter.y;
		closestPoint.y = distance;

		distance = relCenter.z;
		distance = (distance > box.extents.z) ? box.extents.z : (distance < -box.extents.z) ? -box.extents.z : relCenter.z;
		closestPoint.z = distance;

		distance = BMath::Vector3::SquaredMagnitude((closestPoint - relCenter));
		
		if (distance > sphere.radius * sphere.radius)
		{
			return false;
		}

		contactData.collisionNormal = closestPoint - spherePosition;
		contactData.collisionNormal.Normalize();
		contactData.collisionPoint = closestPoint;
		contactData.penetrationDepth = sphere.radius - std::sqrt(distance);

		return true;
	}

	// https://github.com/idmillington/cyclone-physics/blob/master/src/collide_fine.cpp#L311
	bool NarrowOBBOBBCollision(const OBBCollider box1, const OBBCollider box2, ContactData& contactData)
	{
		real rBox1, rBox2;

		BMath::Matrix3x3 rotationMatrix{};
		BMath::Matrix3x3 absRotationMatrix{};

		int bestIndex = -1;
		int DEBUG_bestIndex = -1;

		real smallestPenetration = std::numeric_limits<real>::max();
		real DEBUG_smallestPenetration = std::numeric_limits<real>::max();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				rotationMatrix.data[i][j] = BMath::Vector3::Dot(box1.orientation[i], box2.orientation[j]);
			}
		}

		BMath::Vector3 dirVec = box2.position - box1.position;
		dirVec = BMath::Vector3(BMath::Vector3::Dot(dirVec, box1.orientation[0]), BMath::Vector3::Dot(dirVec, box1.orientation[1]), BMath::Vector3::Dot(dirVec, box1.orientation[2]));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				absRotationMatrix.data[i][j] = std::abs(rotationMatrix.data[i][j]) + 1e-4;
			}
		}

		// Check if vertex of box2 is intersecting with face of box1
		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.extents[i];
			rBox2 = box2.extents[0] * absRotationMatrix.data[i][0] + box2.extents[1] * absRotationMatrix.data[i][1] + box2.extents[2] * absRotationMatrix.data[i][2];
			if (std::abs(dirVec[i]) > (rBox1 + rBox2))
			{
				return false;
			}
			else
			{
				SetSATBestPen(bestIndex, smallestPenetration, std::abs(dirVec[i]), (rBox1 + rBox2), i);
			}
		}

		real absDistance = 0.0f;

		// Check if vertex of box1 is intersecting with face of box2
		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.extents[0] * absRotationMatrix.data[0][i] + box1.extents[1] * absRotationMatrix.data[1][i] + box1.extents[2] * absRotationMatrix.data[2][i];
			rBox2 = box2.extents[i];
			absDistance = std::abs(dirVec[0] * rotationMatrix.data[0][i] + dirVec[1] * rotationMatrix.data[1][i] + dirVec[2] * rotationMatrix.data[2][i]);
			if (absDistance > rBox1 + rBox2)
			{
				return false;
			}
			else
			{
				SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), i + 3);
			}
		}

		rBox1 = box1.extents[1] * absRotationMatrix.data[2][0] + box1.extents[2] * absRotationMatrix.data[1][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[0][2] + box2.extents[2] * absRotationMatrix.data[0][1];
		absDistance = std::abs(dirVec[2] * rotationMatrix.data[1][0] - dirVec[1] * rotationMatrix.data[2][0]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 7);
		}
		rBox1 = box1.extents[1] * absRotationMatrix.data[2][1] + box1.extents[2] * absRotationMatrix.data[1][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[0][2] + box2.extents[2] * absRotationMatrix.data[0][0];
		absDistance = std::abs(dirVec[2] * rotationMatrix.data[1][1] - dirVec[1] * rotationMatrix.data[2][1]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 8);
		}

		// Test axis L = A0 x B2
		rBox1 = box1.extents[1] * absRotationMatrix.data[2][2] + box1.extents[2] * absRotationMatrix.data[1][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[0][1] + box2.extents[1] * absRotationMatrix.data[0][0];
		absDistance = std::abs(dirVec[2] * rotationMatrix.data[1][2] - dirVec[1] * rotationMatrix.data[2][2]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 9);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][0] + box1.extents[2] * absRotationMatrix.data[0][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[1][2] + box2.extents[2] * absRotationMatrix.data[1][1];
		absDistance = std::abs(dirVec[0] * rotationMatrix.data[2][0] - dirVec[2] * rotationMatrix.data[0][0]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 10);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][1] + box1.extents[2] * absRotationMatrix.data[0][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[1][2] + box2.extents[2] * absRotationMatrix.data[1][0];
		absDistance = std::abs(dirVec[0] * rotationMatrix.data[2][1] - dirVec[2] * rotationMatrix.data[0][1]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 11);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][2] + box1.extents[2] * absRotationMatrix.data[0][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[1][1] + box2.extents[1] * absRotationMatrix.data[1][0];
		absDistance = std::abs(dirVec[0] * rotationMatrix.data[2][2] - dirVec[2] * rotationMatrix.data[0][2]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 12);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][0] + box1.extents[1] * absRotationMatrix.data[0][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[2][2] + box2.extents[2] * absRotationMatrix.data[2][1];
		absDistance = std::abs(dirVec[1] * rotationMatrix.data[0][0] - dirVec[0] * rotationMatrix.data[1][0]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 13);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][1] + box1.extents[1] * absRotationMatrix.data[0][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[2][2] + box2.extents[2] * absRotationMatrix.data[2][0];
		absDistance = std::abs(dirVec[1] * rotationMatrix.data[0][1] - dirVec[0] * rotationMatrix.data[1][1]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 14);
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][2] + box1.extents[1] * absRotationMatrix.data[0][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[2][1] + box2.extents[1] * absRotationMatrix.data[2][0];
		absDistance = std::abs(dirVec[1] * rotationMatrix.data[0][2] - dirVec[0] * rotationMatrix.data[1][2]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 15);
		}

		int bestSingleAxis = bestIndex;

		const BMath::Vector3 centerPosition = box2.position - box1.position;
		if (bestIndex < 3)
		{
			OBBVertexFaceCollision(box1, box2, centerPosition, contactData, bestIndex, smallestPenetration);
			return true;
		}
		else if (bestIndex < 6)
		{
			OBBVertexFaceCollision(box2, box1, centerPosition * -1.0f, contactData, bestIndex - 3, smallestPenetration);
			return true;
		}
		else
		{
			bestIndex -= 6;

			int axisOneIndex = bestIndex / 3;
			int axisTwoIndex = bestIndex % 3;
			BMath::Vector3 box1Axis = box1.orientation[axisOneIndex];
			BMath::Vector3 box2Axis = box2.orientation[axisTwoIndex];

			BMath::Vector3 axis = BMath::Vector3::Cross(box1Axis, box2Axis).Normalize();

			if (BMath::Vector3::Dot(axis, centerPosition) > 0)
			{
				axis *= 1.0f;
			}

			BMath::Vector3 ptOnOneEdge = box1.extents;
			BMath::Vector3 ptOnTwoEdge = box2.extents;

			for (int i = 0; i < 3; i++)
			{
				if (i == axisOneIndex)
				{
					ptOnOneEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot(box1.orientation[i], axis) > 0)
				{
					ptOnOneEdge[i] = -ptOnOneEdge[i];
				}

				if (i == axisTwoIndex)
				{
					ptOnTwoEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot(box2.orientation[i], axis) < 0)
				{
					ptOnTwoEdge[i] = -ptOnTwoEdge[i];
				}
			}


			BMath::Matrix4x4 box1Transform {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					box1Transform.data[i][j] = box1.orientation[i][j] * box1.extents[i];
				}
			}

			box1Transform._41 = box1.position.x;
			box1Transform._42 = box1.position.y;
			box1Transform._43 = box1.position.z;


			BMath::Matrix4x4 box2Transform {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					box2Transform.data[i][j] = box2.orientation[i][j] * box2.extents[i];
				}
			}

			box2Transform._41 = box2.position.x;
			box2Transform._42 = box2.position.y;
			box2Transform._43 = box2.position.z;

			ptOnOneEdge = BMath::Vector3(box1Transform * BMath::Vector4(ptOnOneEdge, 1.0f));
			ptOnTwoEdge = BMath::Vector3(box2Transform * BMath::Vector4(ptOnTwoEdge, 1.0f));

			BMath::Vector3 vertex = CalculateOBBContactPoint(
				ptOnOneEdge,
				box1Axis,
				box1.extents[axisOneIndex],
				ptOnTwoEdge,
				box2Axis, box2.extents[axisTwoIndex],
				bestSingleAxis > 2);

			contactData.penetrationDepth = smallestPenetration;
			contactData.collisionNormal = axis;
			contactData.collisionPoint = vertex;

			return true;
		}
	}

	void SetSATBestPen(int& bestIndex, real& bestPen, real absDistance, real combinedBoxes, int index)
	{
		real pen = combinedBoxes - absDistance;

		if (pen < 1e-2) // is parallel
		{
			return;
		}

		if (bestIndex < 0)
		{
			bestIndex = index;
			bestPen = pen;
		}
		else if (pen < bestPen)
		{
			bestIndex = index;
			bestPen = pen;
		}
	}

	void OBBVertexFaceCollision(const OBBCollider& box1, const OBBCollider& box2, const BMath::Vector3& toCenter, ContactData& contactData, int bestIndex, real pen)
	{
		BMath::Vector3 normal = box1.orientation[bestIndex];
		if (BMath::Vector3::Dot(normal, toCenter) > 0.0f)
		{
			normal *= -1.0f;
		}

		BMath::Vector3 collidingVertex = box2.extents / 2.0f;
		if (BMath::Vector3::Dot(box2.orientation[0], normal) < 0.0f)
		{
			collidingVertex.x *= -1.0f;
		}
		if (BMath::Vector3::Dot(box2.orientation[1], normal) < 0.0f)
		{
			collidingVertex.y *= -1.0f;
		}
		if (BMath::Vector3::Dot(box2.orientation[2], normal) < 0.0f)
		{
			collidingVertex.z *= -1.0f;
		}

		BMath::Matrix4x4 transform {};

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transform.data[i][j] = box2.orientation[i][j] * box2.extents[i];
			}
		}

		transform._41 = box2.position.x;
		transform._42 = box2.position.y;
		transform._43 = box2.position.z;

		contactData.collisionNormal = normal;
		contactData.collisionPoint = BMath::Vector3(transform * BMath::Vector4(collidingVertex, 1.0f));
		contactData.penetrationDepth = pen;
	}

	BMath::Vector3 CalculateOBBContactPoint(
		const BMath::Vector3& pOne,
		const BMath::Vector3& dOne,
		real oneSize,
		const BMath::Vector3& pTwo,
		const BMath::Vector3& dTwo,
		real twoSize,
		const bool useOne)
	{
		BMath::Vector3 toSt, cOne, cTwo;
		real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
		real denom, mua, mub;

		smOne = BMath::Vector3::SquaredMagnitude(dOne);
		smTwo = BMath::Vector3::SquaredMagnitude(dTwo);
		dpOneTwo = BMath::Vector3::Dot(dTwo, dOne);

		toSt = pOne - pTwo;
		dpStaOne = BMath::Vector3::Dot(dOne, toSt);
		dpStaTwo = BMath::Vector3::Dot(dTwo, toSt);

		denom = smOne * smTwo - dpOneTwo * dpOneTwo;

		// Zero denominator indicates parrallel lines
		if (std::abs(denom) < EPSILON) 
		{
			return useOne ? pOne : pTwo;
		}

		mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
		mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;


		if (mua > oneSize || mua < -oneSize || mub > twoSize ||mub < -twoSize)
		{
			return useOne ? pOne : pTwo;
		}
		else
		{
			cOne = pOne + dOne * mua;
			cTwo = pTwo + dTwo * mub;

			return cOne * 0.5 + cTwo * 0.5;
		}

	}

	bool TryAxis(
		const OBBCollider& box1,
		const OBBCollider& box2,
		BMath::Vector3 axis,
		const BMath::Vector3 toCenter,
		int index,
		real& smallestPen,
		int& smallestCase)
	{
		if (BMath::Vector3::SquaredMagnitude(axis) < EPSILON)
		{
			return true;
		}

		axis.Normalize();
		real pen = penetrationOnAxis(box1, box2, axis, toCenter);

		if (pen < 0)
		{
			return false;
		}

		if (pen < smallestPen)
		{
			smallestPen = pen;
			smallestCase = index;
		}
		return true;
	}

	real penetrationOnAxis(
		const OBBCollider& box1,
		const OBBCollider& box2,
		BMath::Vector3 axis,
		const BMath::Vector3& toCenter)
	{
		real b1 = box1.extents.x + std::abs(BMath::Vector3::Dot(axis, box1.orientation[0])) +
				   box1.extents.y + std::abs(BMath::Vector3::Dot(axis, box1.orientation[1])) +
				   box1.extents.z + std::abs(BMath::Vector3::Dot(axis, box1.orientation[2]));

		real b2 = box2.extents.x + std::abs(BMath::Vector3::Dot(axis, box2.orientation[0])) +
				   box2.extents.y + std::abs(BMath::Vector3::Dot(axis, box2.orientation[1])) +
				   box2.extents.z + std::abs(BMath::Vector3::Dot(axis, box2.orientation[2]));

		real distance = std::abs(BMath::Vector3::Dot(toCenter, axis));

		return b1 + b2 - distance;
	}
// 
// 
// 
// 
// 	bool DEBUG_tryAxis(
// 		const OBBCollider& one,
// 		const OBBCollider& two,
// 		BMath::Vector3 axis,
// 		const BMath::Vector3& toCentre,
// 		unsigned index,
// 
// 		// These values may be updated
// 		real& smallestPenetration,
// 		unsigned& smallestCase
// 	)
// 	{
// 		// Make sure we have a normalized axis, and don't check almost parallel axes
// 		if (BMath::Vector3::SquaredMagnitude(axis) < 0.0001) 
// 			return true;
// 		axis.Normalize();
// 
// 		real penetration = penetrationOnAxis(one, two, axis, toCentre);
// 
// 		if (penetration < 0) return false;
// 		if (penetration < smallestPenetration) {
// 			smallestPenetration = penetration;
// 			smallestCase = index;
// 		}
// 		return true;
// 	}
// 
// 	// FOR TESTING ONLY DELETE BEFORE SUBMISSION 
// #define CHECK_OVERLAP(axis, index) \
//     if (!DEBUG_tryAxis(one, two, (axis), toCentre, (index), pen, best)) return false;
// 
// 	bool DEBUG_boxAndBox(
// 		const OBBCollider& one,
// 		const OBBCollider& two,
// 		ContactData contactData
// 	)
// 	{
// 		//if (!IntersectionTests::boxAndBox(one, two)) return 0;
// 
// 		// Find the vector between the two centres
// 		BMath::Vector3 toCentre = two.orientation[3] - one.orientation[3];
// 
// 		// We start assuming there is no contact
// 		real pen = std::numeric_limits<real>::max();
// 		unsigned best = 0xffffff;
// 
// 		// Now we check each axes, returning if it gives us
// 		// a separating axis, and keeping track of the axis with
// 		// the smallest penetration otherwise.
// 		CHECK_OVERLAP(one.orientation[0], 0);
// 		CHECK_OVERLAP(one.orientation[1], 1);
// 		CHECK_OVERLAP(one.orientation[2], 2);
// 
// 		CHECK_OVERLAP(two.orientation[0], 3);
// 		CHECK_OVERLAP(two.orientation[1], 4);
// 		CHECK_OVERLAP(two.orientation[2], 5);
// 
// 		// Store the best axis-major, in case we run into almost
// 		// parallel edge collisions later
// 		unsigned bestSingleAxis = best;
// 
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[0], two.orientation[0]), 6);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[0], two.orientation[1]), 7);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[0], two.orientation[2]), 8);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[1], two.orientation[0]), 9);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[1], two.orientation[1]), 10);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[1], two.orientation[2]), 11);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[2], two.orientation[0]), 12);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[2], two.orientation[1]), 13);
// 		CHECK_OVERLAP(BMath::Vector3::Cross(one.orientation[2], two.orientation[2]), 14);
// 
// 		// Make sure we've got a result.
// 		assert(best != 0xffffff);
// 
// 		// We now know there's a collision, and we know which
// 		// of the axes gave the smallest penetration. We now
// 		// can deal with it in different ways depending on
// 		// the case.
// 		if (best < 3)
// 		{
// 			// We've got a vertex of box two on a face of box one.
// 			OBBVertexFaceCollision(one, two, toCentre, contactData, best, pen);
// 			return 1;
// 		}
// 		else if (best < 6)
// 		{
// 			// We've got a vertex of box one on a face of box two.
// 			// We use the same algorithm as above, but swap around
// 			// one and two (and therefore also the vector between their
// 			// centres).
// 			OBBVertexFaceCollision(two, one, toCentre * -1.0f, contactData, best - 3, pen);
// 			return 1;
// 		}
// 		else
// 		{
// 			// We've got an edge-edge contact. Find out which axes
// 			best -= 6;
// 			unsigned oneAxisIndex = best / 3;
// 			unsigned twoAxisIndex = best % 3;
// 			BMath::Vector3 oneAxis = one.orientation[oneAxisIndex];
// 			BMath::Vector3 twoAxis = two.orientation[twoAxisIndex];
// 			BMath::Vector3 axis = BMath::Vector3::Cross(oneAxis, twoAxis);
// 			axis.Normalize();
// 
// 			// The axis should point from box one to box two.
// 			if (BMath::Vector3::Dot(axis, toCentre) > 0) axis = axis * -1.0f;
// 
// 			// We have the axes, but not the edges: each axis has 4 edges parallel
// 			// to it, we need to find which of the 4 for each object. We do
// 			// that by finding the point in the centre of the edge. We know
// 			// its component in the direction of the box's collision axis is zero
// 			// (its a mid-point) and we determine which of the extremes in each
// 			// of the other axes is closest.
// 			BMath::Vector3 ptOnOneEdge = one.extents;
// 			BMath::Vector3 ptOnTwoEdge = two.extents;
// 			for (unsigned i = 0; i < 3; i++)
// 			{
// 				if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
// 				else if (BMath::Vector3::Dot(one.orientation[i], axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];
// 
// 				if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
// 				else if (BMath::Vector3::Dot(two.orientation[i], axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
// 			}
// 
// 			// Move them into world coordinates (they are already oriented
// 			// correctly, since they have been derived from the axes).
// 			BMath::Matrix4x4 box1Transform {};
// 
// 			for (int i = 0; i < 3; i++)
// 			{
// 				for (int j = 0; j < 3; j++)
// 				{
// 					box1Transform.data[i][j] = one.orientation[i][j] * one.extents[i];
// 				}
// 			}
// 
// 			box1Transform._41 = one.position.x;
// 			box1Transform._42 = one.position.y;
// 			box1Transform._43 = one.position.z;
// 
// 
// 			BMath::Matrix4x4 box2Transform {};
// 
// 			for (int i = 0; i < 3; i++)
// 			{
// 				for (int j = 0; j < 3; j++)
// 				{
// 					box2Transform.data[i][j] = two.orientation[i][j] * two.extents[i];
// 				}
// 			}
// 
// 			box2Transform._41 = two.position.x;
// 			box2Transform._42 = two.position.y;
// 			box2Transform._43 = two.position.z;
// 
// 			ptOnOneEdge = BMath::Vector3(box1Transform * BMath::Vector4(ptOnOneEdge, 1.0f));
// 			ptOnTwoEdge = BMath::Vector3(box2Transform * BMath::Vector4(ptOnTwoEdge, 1.0f));
// 
// 			// So we have a point and a direction for the colliding edges.
// 			// We need to find out point of closest approach of the two
// 			// line-segments.
// 			BMath::Vector3 vertex = CalculateOBBContactPoint(
// 				ptOnOneEdge, oneAxis, one.extents[oneAxisIndex],
// 				ptOnTwoEdge, twoAxis, two.extents[twoAxisIndex],
// 				bestSingleAxis > 2
// 			);
// 
// 			// We can fill the contact.
// 		
// 
// 			contactData.penetrationDepth= pen;
// 			contactData.collisionNormal = axis;
// 			contactData.collisionPoint = vertex;
// 
// 			return 1;
// 		}
// 		return 0;
// 	}

}