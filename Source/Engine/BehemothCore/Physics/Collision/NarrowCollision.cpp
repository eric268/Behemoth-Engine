#include "pch.h"
#include "NarrowCollision.h"
#include "BroadCollision.h"
#include "Math/Matrix.h"
#include "Physics/Collision/Colliders.h"
#include "Geometry/Plane.h"
#include "CollisionData.h"
#include "Core/Log.h"

namespace Behemoth
{
	bool NarrowSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box, ContactData& contactData)
	{
		BMath::Vector3 min = box.position - box.extents;
		BMath::Vector3 max = box.position + box.extents;

		BMath::Vector3 closestPoint;
		closestPoint.x = std::max(min.x, std::min(sphere.center.x, max.x));
		closestPoint.y = std::max(min.y, std::min(sphere.center.y, max.y));
		closestPoint.z = std::max(min.z, std::min(sphere.center.z, max.z));

		BMath::Vector3 difference = sphere.center - closestPoint;
		float distance = BMath::Vector3::Magnitude(difference);
		float distanceSquared = distance * distance;

		if (distanceSquared < sphere.radius * sphere.radius)
		{
			contactData.collisionPoint = closestPoint;
			contactData.collisionNormal = difference.Normalize();
			contactData.depth = sphere.radius - distance;
			return true;
		}
		return false;
	}

	bool NarrowRayOBBCollision(const Ray& ray, const OBBCollider& box, ContactData& contactData)
	{
		real collisionStartTime = (real)0.0;
		real collisionEndTime = std::numeric_limits<real>::max();

		BMath::Vector3 collisionNormal;
		BMath::Vector3 distanceCenterRayOrigin = box.position - ray.origin;

		for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
		{
			BMath::Vector3 currentAxis = box.orientation[axisIndex];
			real extentProjection = BMath::Vector3::Dot<real>(currentAxis, distanceCenterRayOrigin);
			real directionProjection = BMath::Vector3::Dot<real>(currentAxis, ray.direction);

			if (abs(directionProjection) < EPSILON)
			{
				if (-extentProjection - box.extents[axisIndex] > 0 || -extentProjection + box.extents[axisIndex] < 0)
				{
					return false;
				}
			}
			else
			{
				real entryPointTime = (extentProjection + box.extents[axisIndex]) / directionProjection;
				real exitPointTime = (extentProjection - box.extents[axisIndex]) / directionProjection;

				if (entryPointTime > exitPointTime)
				{
					std::swap(entryPointTime, exitPointTime);
				}

				if (entryPointTime > collisionStartTime) {
					collisionStartTime = entryPointTime;
					collisionNormal = currentAxis * ((extentProjection + box.extents[axisIndex]) > 0 ? -1 : 1);
				}

				collisionEndTime = std::min(collisionEndTime, exitPointTime);

				if (collisionStartTime > collisionEndTime || collisionEndTime < 0) 
				{
					return false;
				}
			}
		}
		contactData.depth = collisionStartTime;
		contactData.collisionNormal = collisionNormal.Normalize();
		contactData.collisionPoint = ray.origin + ray.direction * collisionStartTime;
		return true;
	}

	bool NarrowRaySphereCollision(const Ray& ray, const SphereCollider& sphere, ContactData& contactData)
	{
		BMath::Vector3 rayToSphereVector = ray.origin - sphere.center;
		real directionMagnitudeSquared = BMath::Vector3::Dot<real>(ray.direction, ray.direction);
		real projectionOntoRay = 2.0f * BMath::Vector3::Dot<real>(ray.direction, rayToSphereVector);
		real sphereEquationConstant = BMath::Vector3::Dot<real>(rayToSphereVector, rayToSphereVector) - sphere.radius * sphere.radius;

		real discriminant = projectionOntoRay * projectionOntoRay - 4 * directionMagnitudeSquared * sphereEquationConstant;

		if (discriminant < 0)
		{
			return false;
		}

		real sqrtDiscriminant = std::sqrt(discriminant);

		real entryTime = (-projectionOntoRay - sqrtDiscriminant) / ((real)2.0 * directionMagnitudeSquared);
		real exitTime = (-projectionOntoRay + sqrtDiscriminant) / ((real)2.0 * directionMagnitudeSquared);

		real collisionTime = (entryTime < exitTime) ? entryTime : exitTime;

		if (collisionTime < 0)
		{
			collisionTime = exitTime;
			if (collisionTime < 0)
			{
				return false;
			}
		}

		contactData.depth = collisionTime;
		contactData.collisionPoint = ray.origin + ray.direction * collisionTime;
		contactData.collisionNormal = (contactData.collisionPoint - sphere.center) / sphere.radius;

		return true;
	}


	bool NarrowSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2, ContactData& contactData)
	{
		BMath::Vector3 positionOne = sphere1.center;
		BMath::Vector3 positionTwo = sphere2.center;

		BMath::Vector3 midline = positionOne - positionTwo;
		real size = BMath::Vector3::Magnitude(midline);

		if (size <= (real)0.0 || size > sphere1.radius + sphere2.radius)
		{
			return false;
		}

		BMath::Vector3 normal = midline / size;
		contactData.collisionNormal = normal;
		contactData.collisionPoint = positionOne + midline * (real)0.5;
		contactData.depth = (sphere1.radius + sphere2.radius - size);
		return true;
	}

	bool NarrowSpherePlaneCollision(const SphereCollider& sphere, const Plane& plane, ContactData& contactData)
	{
		BMath::Vector3 spherePosition = sphere.center;
		real distanceFromPlane = BMath::Vector3::Dot<real>(plane.normal, spherePosition) - sphere.radius - Plane::CalculatePlaneOffset(plane.normal, Plane::GetPointOnPlane(plane));

		if (distanceFromPlane >= 0.0f)
		{
			return false;
		}

		contactData.collisionNormal = plane.normal;
		contactData.depth = -distanceFromPlane;
		contactData.collisionPoint = spherePosition - plane.normal * (distanceFromPlane + sphere.radius);
		return true;
	}

	bool NarrowOBBPlaneCollision(const OBBCollider& box, const Plane& plane, std::vector<ContactData>& data)
	{
		if (!BroadOBBPlaneCollision(box, plane))
		{
			return false;
		}

		static real mults[8][3] =
		{
			{1,1,1},  {-1,1,1},  {1,-1,1},   {-1,-1, 1},
			{1,1,-1}, {-1,1,-1}, {1,-1,-1},  {-1,-1,-1}
		};

		const real planeOffset = Plane::CalculatePlaneOffset(plane.normal, Plane::GetPointOnPlane(plane));

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

			real vertexDistance = BMath::Vector3::Dot(vertexPosition, plane.normal);

			// Vertex is penetrating plane
			if (vertexDistance <= planeOffset)
			{
				collisionOccured = true;
				ContactData contactData{};
				contactData.collisionNormal = plane.normal;
				contactData.depth = planeOffset - vertexDistance;
				contactData.collisionPoint = vertexPosition - plane.normal * (vertexDistance - planeOffset) * (real)0.5;

				// Decide later if we want to save all contacts or break out early 
				data.push_back(contactData);
			}
		}

		return collisionOccured;
	}

	bool NarrowOBBSphereCollision(const OBBCollider& box, const SphereCollider& sphere, ContactData& contactData)
	{
		BMath::BMatrix3x3 rot = BMath::BMatrix3x3::Identity();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				rot.data[i][j] = box.orientation[i][j];
			}
		}

		BMath::Vector3 localToSphere = BMath::Vector3(BMath::BMatrix3x3::Inverse(rot) * BMath::Vector3(sphere.center - box.position));

		BMath::Vector3 closestPoint(0.0f);

		for (int i = 0; i < 3; ++i)
		{
			real value = localToSphere[i];
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

		BMath::Vector3 worldClosestPoint = rot * closestPoint + box.position;

		BMath::Vector3 toSphere = sphere.center - worldClosestPoint;

		float distanceSquared = BMath::Vector3::Dot(toSphere, toSphere);

		if (distanceSquared > sphere.radius * sphere.radius)
		{
			return false;
		}

		float distance = std::sqrt(distanceSquared);
		contactData.collisionNormal = BMath::Vector3::Normalize(toSphere.Normalize());
		contactData.collisionPoint = worldClosestPoint;
		contactData.depth = sphere.radius - distance;
		return true;
	}



	// https://github.com/idmillington/cyclone-physics/blob/master/src/collide_fine.cpp#L311
	bool NarrowSphereOBBCollision(const SphereCollider& sphere, const OBBCollider& box, ContactData& collisionData)
	{
		bool result = NarrowOBBSphereCollision(box, sphere, collisionData);
		// If we are checking sphere against OBB instead of the OBB against sphere, we have to invert the normal
		collisionData.collisionNormal *= -1.0f;
		return result;
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

		BMath::Vector3 collidingVertex = box2.extents / 2.0f;
		if (BMath::Vector3::Dot<real>(box2.orientation[0], normal) < (real)0.0)
		{
			collidingVertex.x *= (real)-1.0;
		}
		if (BMath::Vector3::Dot<real>(box2.orientation[1], normal) < (real)0.0)
		{
			collidingVertex.y *= (real) - 1.0f;
		}
		if (BMath::Vector3::Dot<real>(box2.orientation[2], normal) < (real)0.0f)
		{
			collidingVertex.z *= (real) - 1.0f;
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
		contactData.depth = pen;
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

		smOne = BMath::Vector3::SquaredMagnitude<real>(dOne);
		smTwo = BMath::Vector3::SquaredMagnitude<real>(dTwo);
		dpOneTwo = BMath::Vector3::Dot<real>(dTwo, dOne);

		toSt = pOne - pTwo;
		dpStaOne = BMath::Vector3::Dot<real>(dOne, toSt);
		dpStaTwo = BMath::Vector3::Dot<real>(dTwo, toSt);

		denom = smOne * smTwo - dpOneTwo * dpOneTwo;

		// Zero denominator indicates parrallel lines
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

			return cOne * (real) 0.5 + cTwo * (real)0.5;
		}

	}

	bool NarrowOBBOBBCollision(const OBBCollider box1, const OBBCollider box2, ContactData& contactData)
	{
		real rBox1, rBox2;

		BMath::BMatrix3x3d rotationMatrix{};
		BMath::BMatrix3x3d absRotationMatrix{};

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
				absRotationMatrix.data[i][j] = std::abs(rotationMatrix.data[i][j]) + 1e-5;
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

			contactData.depth = smallestPenetration;
			contactData.collisionNormal = axis.Normalize();
			contactData.collisionPoint = vertex;

			return true;
		}
	}

}