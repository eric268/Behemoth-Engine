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

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, CollisionData& collisionData)
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
			BMath::Vector3 vertexPosition = BMath::Vector3(mults[i][0] * box.worldExtents.x, mults[i][1] * box.worldExtents.y, mults[i][2] * box.worldExtents.z);
			
			vertexPosition = BMath::Vector3
			(
				box.worldOrientation[0].x * vertexPosition.x + box.worldOrientation[1].x * vertexPosition.y + box.worldOrientation[2].x * vertexPosition.z,
				box.worldOrientation[0].y * vertexPosition.x + box.worldOrientation[1].y * vertexPosition.y + box.worldOrientation[2].y * vertexPosition.z,
				box.worldOrientation[0].z * vertexPosition.x + box.worldOrientation[1].z * vertexPosition.y + box.worldOrientation[2].z * vertexPosition.z
			);

			vertexPosition += box.worldPosition;

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
				collisionData.collisionContacts.push_back(contactData);
			}
		}

		return collisionOccured;
	}

	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& contactData)
	{
		BMath::Vector3 spherePosition = sphere.position;
		BMath::Vector3 boxPosition = box.worldPosition;

		BMath::Vector3 translatedCenter = spherePosition - boxPosition;

		// Transpose the rotation matrix formed by the orientation vectors
		BMath::Vector3 invRotatedCenter = BMath::Vector3(
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.worldOrientation[0].x,box.worldOrientation[1].x, box.worldOrientation[2].x)),
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.worldOrientation[0].y,box.worldOrientation[1].y, box.worldOrientation[2].y)),
			BMath::Vector3::Dot(translatedCenter, BMath::Vector3(box.worldOrientation[0].z,box.worldOrientation[1].z, box.worldOrientation[2].z))
		);

		BMath::Vector3 relCenter = BMath::Vector3
		(
			invRotatedCenter.x / box.worldExtents.x,
			invRotatedCenter.y / box.worldExtents.y,
			invRotatedCenter.z / box.worldExtents.z
		);

		if (std::abs(relCenter.x) - sphere.radius > box.worldExtents.x ||
			std::abs(relCenter.y) - sphere.radius > box.worldExtents.y ||
			std::abs(relCenter.z) - sphere.radius > box.worldExtents.z)
		{
			return false;
		}

		BMath::Vector3 closestPoint(0.0f);

		float distance = relCenter.x;
		distance = (distance > box.worldExtents.x) ? box.worldExtents.x : (distance < -box.worldExtents.x) ? -box.worldExtents.x : relCenter.x;
		closestPoint.x = distance;
	
		distance = relCenter.y;
		distance = (distance > box.worldExtents.y) ? box.worldExtents.y : (distance < -box.worldExtents.y) ? -box.worldExtents.y : relCenter.y;
		closestPoint.y = distance;

		distance = relCenter.z;
		distance = (distance > box.worldExtents.z) ? box.worldExtents.z : (distance < -box.worldExtents.z) ? -box.worldExtents.z : relCenter.z;
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
	bool NarrowOBBOBBCollision(const OBBCollider& box1, const OBBCollider& box2, ContactData& contactData)
	{
		float rBox1, rBox2;

		BMath::Matrix3x3 R{};
		BMath::Matrix3x3 AbsR{};

		int bestIndex = -1;
		float smallestPenetration = std::numeric_limits<float>::max();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				R.data[i][j] = BMath::Vector3::Dot(box1.worldOrientation[i], box2.worldOrientation[j]);
			}
		}

		BMath::Vector3 distance = box2.worldPosition - box1.worldPosition;
		distance = BMath::Vector3(BMath::Vector3::Dot(distance, box1.worldOrientation[0]), BMath::Vector3::Dot(distance, box1.worldOrientation[1]), BMath::Vector3::Dot(distance, box1.worldOrientation[2]));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				AbsR.data[i][j] = std::abs(R.data[i][j]) + EPSILON;
			}
		}

		// Check if vertex of box2 is intersecting with face of box1
		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.worldExtents[i];
			rBox2 = box2.worldExtents[0] * AbsR.data[i][0] + box2.worldExtents[1] * AbsR.data[i][1] + box2.worldExtents[2] * AbsR.data[i][2];
			if (std::abs(distance[i]) > (rBox1 + rBox2))
			{
				return false;
			}
			else
			{
				SetSATBestPen(bestIndex, smallestPenetration, std::abs(distance[i]), (rBox1 + rBox2), i);
			}
		}

		float absDistance = 0.0f;

		// Check if vertex of box1 is intersecting with face of box2
		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.worldExtents[0] * AbsR.data[0][i] + box1.worldExtents[1] * AbsR.data[1][i] + box1.worldExtents[2] * AbsR.data[2][i];
			rBox2 = box2.worldExtents[i];
			absDistance = std::abs(distance[0] * R.data[0][i] + distance[1] * R.data[1][i] + distance[2] * R.data[2][i]);
			if (absDistance > (rBox1 + rBox2))
			{
				return false;
			}
			else
			{
				SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), i + 3);
			}
		}

		rBox1 = box1.worldExtents[1] * AbsR.data[2][0] + box1.worldExtents[2] * AbsR.data[1][0];
		rBox2 = box2.worldExtents[1] * AbsR.data[0][2] + box2.worldExtents[2] * AbsR.data[0][1];

		absDistance = std::abs(distance[2] + R.data[1][0] - distance[1] * R.data[2][0]);
		if (absDistance > rBox1 + rBox2)
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 7);
		}

		rBox1 = box1.worldExtents[1] * AbsR.data[2][1] + box1.worldExtents[2] * AbsR.data[1][1];
		rBox2 = box2.worldExtents[0] * AbsR.data[0][2] + box2.worldExtents[2] * AbsR.data[0][0];
		if (std::abs(distance[2] + R.data[1][1] - distance[1] * R.data[2][1] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 8);
		}

		rBox1 = box1.worldExtents[1] * AbsR.data[2][2] + box1.worldExtents[2] * AbsR.data[1][2];
		rBox2 = box2.worldExtents[0] * AbsR.data[0][1] + box2.worldExtents[1] * AbsR.data[0][0];
		if (std::abs(distance[2] + R.data[1][2] - distance[1] * R.data[2][2] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 9);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[2][0] + box1.worldExtents[2] * AbsR.data[0][0];
		rBox2 = box2.worldExtents[1] * AbsR.data[1][2] + box2.worldExtents[2] * AbsR.data[1][1];
		if (std::abs(distance[0] + R.data[2][0] - distance[2] * R.data[0][0] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 10);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[2][1] + box1.worldExtents[2] * AbsR.data[0][1];
		rBox2 = box2.worldExtents[0] * AbsR.data[1][2] + box2.worldExtents[2] * AbsR.data[1][0];
		if (std::abs(distance[0] + R.data[2][1] - distance[2] * R.data[0][1] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 11);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[2][2] + box1.worldExtents[2] * AbsR.data[0][2];
		rBox2 = box2.worldExtents[0] * AbsR.data[1][1] + box2.worldExtents[1] * AbsR.data[1][0];
		if (std::abs(distance[0] + R.data[2][2] - distance[2] * R.data[0][2] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 12);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[1][0] + box1.worldExtents[1] * AbsR.data[0][0];
		rBox2 = box2.worldExtents[1] * AbsR.data[2][2] + box2.worldExtents[2] * AbsR.data[2][1];
		if (std::abs(distance[1] + R.data[0][0] - distance[0] * R.data[1][0] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 13);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[1][1] + box1.worldExtents[1] * AbsR.data[0][1];
		rBox2 = box2.worldExtents[0] * AbsR.data[2][2] + box2.worldExtents[2] * AbsR.data[2][0];
		if (std::abs(distance[1] + R.data[0][1] - distance[0] * R.data[1][1] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 14);
		}

		rBox1 = box1.worldExtents[0] * AbsR.data[1][2] + box1.worldExtents[1] * AbsR.data[0][2];
		rBox2 = box2.worldExtents[0] * AbsR.data[2][1] + box2.worldExtents[1] * AbsR.data[2][0];
		if (std::abs(distance[1] + R.data[0][2] - distance[0] * R.data[1][2] > rBox1 + rBox2))
		{
			return false;
		}
		else
		{
			SetSATBestPen(bestIndex, smallestPenetration, absDistance, (rBox1 + rBox2), 15);
		}

		int bestSingleAxis = bestIndex;

		const BMath::Vector3 centerPosition = box2.worldPosition - box1.worldPosition;
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
			BMath::Vector3 box1Axis = box1.worldOrientation[axisOneIndex];
			BMath::Vector3 box2Axis = box1.worldOrientation[axisTwoIndex];

			BMath::Vector3 axis = BMath::Vector3::Cross(box1Axis, box2Axis).Normalize();

			if (BMath::Vector3::Dot(axis, centerPosition) > 0)
			{
				axis *= 1.0f;
			}

			BMath::Vector3 ptOnOneEdge = box1.worldExtents;
			BMath::Vector3 ptOnTwoEdge = box1.worldExtents;

			for (int i = 0; i < 3; i++)
			{
				if (i == axisOneIndex)
				{
					ptOnOneEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot(box1.worldOrientation[i], axis) > 0)
				{
					ptOnOneEdge[i] = -ptOnOneEdge[i];
				}

				if (i == axisTwoIndex)
				{
					ptOnTwoEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot(box2.worldOrientation[i], axis) < 0)
				{
					ptOnTwoEdge[i] = -ptOnTwoEdge[i];
				}
			}


			BMath::Matrix4x4 box1Transform {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					box1Transform.data[i][j] = box1.worldOrientation[i][j] * box1.worldExtents[i];
				}
			}

			box1Transform._41 = box1.worldPosition.x;
			box1Transform._42 = box1.worldPosition.y;
			box1Transform._43 = box1.worldPosition.z;


			BMath::Matrix4x4 box2Transform {};

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					box2Transform.data[i][j] = box2.worldOrientation[i][j] * box2.worldExtents[i];
				}
			}

			box2Transform._41 = box2.worldPosition.x;
			box2Transform._42 = box2.worldPosition.y;
			box2Transform._43 = box2.worldPosition.z;

			ptOnOneEdge = BMath::Vector3(box1Transform * BMath::Vector4(ptOnOneEdge, 1.0f));
			ptOnTwoEdge = BMath::Vector3(box2Transform * BMath::Vector4(ptOnTwoEdge, 1.0f));

			BMath::Vector3 vertex = CalculateOBBContactPoint(
				ptOnOneEdge,
				box1Axis,
				box1.worldExtents[axisOneIndex],
				ptOnTwoEdge,
				box2Axis, box2.worldExtents[axisTwoIndex],
				bestSingleAxis > 2);

			contactData.penetrationDepth = smallestPenetration;
			contactData.collisionNormal = axis;
			contactData.collisionPoint = vertex;

			return true;
		}
	}

	void SetSATBestPen(int& bestIndex, float& bestPen, float absDistance, float combinedBoxes, int index)
	{
		float pen = combinedBoxes - absDistance;

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

	void OBBVertexFaceCollision(const OBBCollider& box1, const OBBCollider& box2, const BMath::Vector3& toCenter, ContactData& contactData, int bestIndex, float pen)
	{
		BMath::Vector3 normal = box1.worldOrientation[bestIndex];
		if (BMath::Vector3::Dot(normal, toCenter) > 0.0f)
		{
			normal *= -1.0f;
		}

		BMath::Vector3 collidingVertex = box2.worldExtents / 2.0f;
		if (BMath::Vector3::Dot(box2.worldOrientation[0], normal) < 0.0f)
		{
			collidingVertex.x *= -1.0f;
		}
		if (BMath::Vector3::Dot(box2.worldOrientation[1], normal) < 0.0f)
		{
			collidingVertex.y *= -1.0f;
		}
		if (BMath::Vector3::Dot(box2.worldOrientation[2], normal) < 0.0f)
		{
			collidingVertex.z *= -1.0f;
		}

		BMath::Matrix4x4 transform {};

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transform.data[i][j] = box2.worldOrientation[i][j] * box2.worldExtents[i];
			}
		}

		transform._41 = box2.worldPosition.x;
		transform._42 = box2.worldPosition.y;
		transform._43 = box2.worldPosition.z;

		contactData.collisionNormal = normal;
		contactData.collisionPoint = BMath::Vector3(transform * BMath::Vector4(collidingVertex, 1.0f));
		contactData.penetrationDepth = pen;
	}

	BMath::Vector3 CalculateOBBContactPoint(
		const BMath::Vector3& pOne,
		const BMath::Vector3& dOne,
		float oneSize,
		const BMath::Vector3& pTwo,
		const BMath::Vector3& dTwo,
		float twoSize,
		const bool useOne)
	{
		BMath::Vector3 toSt, cOne, cTwo;
		float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
		float denom, mua, mub;

		smOne = BMath::Vector3::SquaredMagnitude(dOne);
		smTwo = BMath::Vector3::SquaredMagnitude(dTwo);

		dpOneTwo = BMath::Vector3::Dot(dTwo, dOne);

		toSt = pOne - pTwo;
		dpStaOne = BMath::Vector3::Dot(dOne, toSt);
		dpStaTwo = BMath::Vector3::Dot(dTwo, toSt);

		denom = smOne * smTwo - dpOneTwo * dpOneTwo;

		// 0 denominator means parrel edge lines
		if (std::abs(denom) < EPSILON)
		{
			return useOne ? pOne : pTwo;
		}

		mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
		mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

		if (mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
		{
			return useOne ? pOne : pTwo;
		}
		else
		{
			cOne = pOne + dOne * mua;
			cTwo = pTwo + dTwo * mub;

			return cOne * 0.5f + cTwo * 0.5f;
		}

	}

}