#include "pch.h"
#include "NarrowCollision.h"
#include "BroadCollision.h"
#include "Math/Matrix.h"
#include "Physics/Collision/Colliders.h"
#include "Geometry/Plane.h"
#include "CollisionData.h"
#include "Misc/Log.h"

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
		BMath::Matrix4x4d boxTransform = BMath::Matrix4x4d::Identity();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				boxTransform.data[i][j] = box.orientation[i][j] * box.extents[i];
			}
		}

		boxTransform._41 = box.position.x;
		boxTransform._42 = box.position.y;
		boxTransform._43 = box.position.z;

		BMath::Vector3 localToSphere = BMath::Vector3(BMath::Matrix4x4d::Inverse(boxTransform) * BMath::Vector4(sphere.position, 1.0f));

		BMath::Vector3 closestPoint(0.0f);

		for (int i = 0; i < 3; ++i) 
		{
			float value = localToSphere[i];
			if (value > box.extents[i])
			{
				value = box.extents[i];
			}
			if (value < -box.extents[i])
			{
				value = -box.extents[i];
			}
			closestPoint[i] = value;
		}

		BMath::Vector3 diff = localToSphere - closestPoint;
		float squareDist = BMath::Vector3::SquaredMagnitude(diff);
		
		if (squareDist > sphere.radius * sphere.radius)
		{
			return false;
		}

		BMath::Vector3 worldContactPoint = BMath::Vector3(boxTransform * BMath::Vector4(closestPoint, 1.0f));
		contactData.collisionPoint = worldContactPoint;
		contactData.collisionNormal = diff.Normalize();
		contactData.penetrationDepth = sphere.radius - std::sqrt(squareDist);

		return true;
	}

	// https://github.com/idmillington/cyclone-physics/blob/master/src/collide_fine.cpp#L311
	bool NarrowOBBOBBCollision(const OBBCollider box1, const OBBCollider box2, ContactData& contactData)
	{
		real rBox1, rBox2;
		
		BMath::Matrix3x3d rotationMatrix{};
		BMath::Matrix3x3d absRotationMatrix{};

		int bestIndex = -1;
		int DEBUG_bestIndex = -1;

		real smallestPenetration = std::numeric_limits<real>::max();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				rotationMatrix.data[i][j] = BMath::Vector3::Dot<real>(box1.orientation[i], box2.orientation[j]);
			}
		}

		BMath::Vector3 dirVec = box2.position - box1.position;
		dirVec = BMath::Vector3(BMath::Vector3::Dot<real>(dirVec, box1.orientation[0]), BMath::Vector3::Dot<real>(dirVec, box1.orientation[1]), BMath::Vector3::Dot<real>(dirVec, box1.orientation[2]));

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

		real absDistance = 0.0;

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

		if (bestIndex < 6)
		{
			OBBVertexFaceCollision(box1, box2, centerPosition, contactData, bestIndex % 3, smallestPenetration);
			return true;
		}
		else
		{
			bestIndex -= 6;

			int axisOneIndex = bestIndex / 3;
			int axisTwoIndex = bestIndex % 3;
			BMath::Vector3 box1Axis = box1.orientation[axisOneIndex];
			BMath::Vector3 box2Axis = box2.orientation[axisTwoIndex];
			BMath::Vector3 axis = (box1.position - box2.position).Normalize();

			BMath::Vector3 ptOnOneEdge = box1.extents;
			BMath::Vector3 ptOnTwoEdge = box2.extents;

			for (int i = 0; i < 3; i++)
			{
				if (i == axisOneIndex)
				{
					ptOnOneEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot<real>(box1.orientation[i], axis) > 0)
				{
					ptOnOneEdge[i] = -ptOnOneEdge[i];
				}

				if (i == axisTwoIndex)
				{
					ptOnTwoEdge[i] = 0;
				}
				else if (BMath::Vector3::Dot<real>(box2.orientation[i], axis) < 0)
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
			contactData.collisionNormal = axis.Normalize();
			contactData.collisionPoint = vertex;

			return true;
		}
	}

	void SetSATBestPen(int& bestIndex, real& bestPen, real absDistance, real combinedBoxes, int index)
	{
		real pen = combinedBoxes - absDistance;

		if (pen < 5e-4) // is parallel
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
		
		if (BMath::Vector3::Dot<double>(normal, toCenter) > 0.0f)
		{
			// normal *= -1.0f;
		}

		BMath::Vector3 collidingVertex = box2.extents / 2.0f;
		if (BMath::Vector3::Dot<double>(box2.orientation[0], normal) < 0.0f)
		{
			collidingVertex.x *= -1.0f;
		}
		if (BMath::Vector3::Dot<double>(box2.orientation[1], normal) < 0.0f)
		{
			collidingVertex.y *= -1.0f;
		}
		if (BMath::Vector3::Dot<double>(box2.orientation[2], normal) < 0.0f)
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

		contactData.collisionNormal = normal.Normalize();
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

		smOne = BMath::Vector3::SquaredMagnitude<double>(dOne);
		smTwo = BMath::Vector3::SquaredMagnitude<double>(dTwo);
		dpOneTwo = BMath::Vector3::Dot<double>(dTwo, dOne);

		toSt = pOne - pTwo;
		dpStaOne = BMath::Vector3::Dot<double>(dOne, toSt);
		dpStaTwo = BMath::Vector3::Dot<double>(dTwo, toSt);

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
		real b1 =  box1.extents.x + std::abs(BMath::Vector3::Dot(axis, box1.orientation[0])) +
				   box1.extents.y + std::abs(BMath::Vector3::Dot(axis, box1.orientation[1])) +
				   box1.extents.z + std::abs(BMath::Vector3::Dot(axis, box1.orientation[2]));

		real b2 =  box2.extents.x + std::abs(BMath::Vector3::Dot(axis, box2.orientation[0])) +
				   box2.extents.y + std::abs(BMath::Vector3::Dot(axis, box2.orientation[1])) +
				   box2.extents.z + std::abs(BMath::Vector3::Dot(axis, box2.orientation[2]));

		real distance = std::abs(BMath::Vector3::Dot(toCenter, axis));

		return b1 + b2 - distance;
	}
}

// const BMath::Vector3 spherePosition = sphere.position;
// const BMath::Vector3 boxPosition = box.position;
// 
// BMath::Matrix4x4d boxTransform = BMath::Matrix4x4d::Identity();
// 
// for (int i = 0; i < 3; i++)
// {
// 	for (int j = 0; j < 3; j++)
// 	{
// 		boxTransform.data[i][j] = box.orientation[i][j] * box.extents[i];
// 	}
// }
// 
// boxTransform._41 = box.position.x;
// boxTransform._42 = box.position.y;
// boxTransform._43 = box.position.z;
// 
// BMath::Vector3 rC = spherePosition - boxPosition;
// 
// const BMath::Matrix4x4d invBoxTransform = BMath::Matrix4x4d::Transpose(boxTransform);
// BMath::Vector3 relativeCenter = BMath::Vector3(invBoxTransform * BMath::Vector4(spherePosition, 1.0f));
// 
// 
// 
// BMath::Vector3 translatedCenter = spherePosition - boxPosition;
// 
// BMath::Vector3 closestPoint(0.0f);
// 
// float distance = relativeCenter.x;
// distance = (distance > box.extents.x) ? box.extents.x : (distance < -box.extents.x) ? -box.extents.x : relativeCenter.x;
// closestPoint.x = distance;
// 
// distance = relativeCenter.y;
// distance = (distance > box.extents.y) ? box.extents.y : (distance < -box.extents.y) ? -box.extents.y : relativeCenter.y;
// closestPoint.y = distance;
// 
// distance = relativeCenter.z;
// distance = (distance > box.extents.z) ? box.extents.z : (distance < -box.extents.z) ? -box.extents.z : relativeCenter.z;
// closestPoint.z = distance;
// 
// distance = BMath::Vector3::SquaredMagnitude((relativeCenter - closestPoint));
// 
// if (distance > sphere.radius * sphere.radius)
// {
// 	return false;
// }
// 
// BMath::Vector3 closestPointWorld = BMath::Vector3(invBoxTransform * BMath::Vector4(closestPoint, 1.0f));
// 
// 
// contactData.collisionNormal = (spherePosition - closestPointWorld).Normalize();
// contactData.collisionNormal.Normalize();
// contactData.collisionPoint = closestPointWorld;
// contactData.penetrationDepth = sphere.radius - std::sqrt(distance);
// 
// return true;