#include "pch.h"
#include "BroadCollision.h"
#include "Math/MathCore.h"
#include "Colliders.h"
#include "Physics/Collision/Colliders.h"
#include "Geometry/Plane.h"
#include "Physics/Ray.h"

namespace Behemoth
{
	bool BroadAABBCollision(const AABBCollider& box1, const AABBCollider& box2)
	{
		BMath::Vector3 minPos1 = box1.position - box1.extents;
		BMath::Vector3 maxPos1 = box1.position + box1.extents;

		BMath::Vector3 minPos2 = box2.position - box2.extents;
		BMath::Vector3 maxPos2 = box2.position + box2.extents;


		if (maxPos1.x < minPos2.x || minPos1.x > maxPos2.x)
		{
			return false;
		}
		if (maxPos1.y < minPos2.y || minPos1.y > maxPos2.y)
		{
			return false;
		}
		if (maxPos1.z < minPos2.z || minPos1.z > maxPos2.z)
		{
			return false;
		}

		return true;
	}
	bool BroadAABBPlaneCollision(const AABBCollider& collider, const Plane& p)
	{
		float radius = collider.extents[0] * std::abs(p.normal[0]) + 
					   collider.extents[1] * std::abs(p.normal[1]) + 
					   collider.extents[2] * std::abs(p.normal[2]);

		float distance = BMath::Vector3::Dot(p.normal, collider.position) - p.d;
		return std::abs(distance) <= radius;
	}

	bool BroadSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2)
	{
		float distance = BMath::Vector3::SquaredDistance(sphere1.center, sphere2.center);
		float radius = sphere1.radius + sphere2.radius;
		return distance <= (radius * radius);
	}

	bool BroadSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box)
	{
		float squaredDist = 0.0f;
		float squaredRad = sphere.radius * sphere.radius;

		for (int axis = 0; axis < 3; axis++)
		{
			float diff = 0;
			float spherePos = sphere.center[axis];
			float minVal = box.position[axis] - box.extents[axis];
			float maxVal = box.position[axis] + box.extents[axis];

			if (spherePos < minVal)
			{
				diff = minVal - spherePos;
			}
			else if (spherePos > maxVal)
			{
				diff = spherePos - maxVal;
			}
			squaredDist += diff * diff;
		}
		return squaredDist <= squaredRad;
	}

	bool BroadLinePlaneIntersection(const Point& p1, const Point& p2, const Plane& plane, float& dist, Point& intersectionP)
	{
		BMath::Vector3 ab = p2 - p1;
		dist = (plane.d -BMath::Vector3::Dot(plane.normal, p1)) / (BMath::Vector3::Dot(plane.normal, ab));
		
		if (dist >= 0.0f && dist <= 1.0f)
		{
			intersectionP = p1 + ab * dist;
			return true;
		}
		return false;
	}
	bool BroadLineAABBIntersection(const Point& lineStart, const Point& lineEnd, const AABBCollider& box)
	{
		BMath::Vector3 boxMin = box.position - box.extents;
		BMath::Vector3 boxMax = box.position + box.extents;

		Point boxCenter = (boxMin + boxMax) * 0.5f; 
		BMath::Vector3 boxExtentsHalf = boxMax - boxCenter;
		Point lineMidpoint = (lineStart + lineEnd) * 0.5f;
		BMath::Vector3 lineHalfVector = lineEnd - lineMidpoint; 
		lineMidpoint = lineMidpoint - boxCenter; 

		// Absolute values of the line's half vector components
		float absLineHalfVectorX = std::abs(lineHalfVector.x);
		float absLineHalfVectorY = std::abs(lineHalfVector.y);
		float absLineHalfVectorZ = std::abs(lineHalfVector.z);

		// Check for separation along X axis
		if (std::abs(lineMidpoint.x) > boxExtentsHalf.x + absLineHalfVectorX)
		{
			return false;
		}

		// Check for separation along Y axis
		if (std::abs(lineMidpoint.y) > boxExtentsHalf.y + absLineHalfVectorY)
		{
			return false;
		}

		// Check for separation along Z axis
		if (std::abs(lineMidpoint.z) > boxExtentsHalf.z + absLineHalfVectorZ)
		{
			return false;
		}

		// Add epsilon to avoid floating point errors
		absLineHalfVectorX += EPSILON;
		absLineHalfVectorY += EPSILON;
		absLineHalfVectorZ += EPSILON;

		// Check for cross products of line vector and AABB's face normals
		if (std::abs(lineMidpoint.y * lineHalfVector.z - lineMidpoint.z * lineHalfVector.y) > boxExtentsHalf.y * absLineHalfVectorZ + boxExtentsHalf.z * absLineHalfVectorY)
		{
			return false;
		}
		if (std::abs(lineMidpoint.z * lineHalfVector.x - lineMidpoint.x * lineHalfVector.z) > boxExtentsHalf.x * absLineHalfVectorZ + boxExtentsHalf.z * absLineHalfVectorX)
		{
			return false;
		}
		if (std::abs(lineMidpoint.x * lineHalfVector.y - lineMidpoint.y * lineHalfVector.x) > boxExtentsHalf.x * absLineHalfVectorY + boxExtentsHalf.y * absLineHalfVectorX)
		{
			return false;
		}

		return true;
	}

	// float-Time Collision Detection-Morgan Kaufmann (2005)
	bool BroadOBBCollision(const OBBCollider& box1, const OBBCollider& box2)
	{
		float rBox1, rBox2;
		BMath::Matrix3x3 rotationMatrix, absRotationMatrix;

		// Compute rotation matrix expressing b in a’s coordinate frame
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				rotationMatrix.data[i][j] = BMath::Vector3::Dot(box1.orientation[i], box2.orientation[j]);
			}
		}

		BMath::Vector3 dirVec = box2.position - box1.position;

		dirVec = BMath::Vector3(BMath::Vector3::Dot(dirVec, box1.orientation[0]), BMath::Vector3::Dot(dirVec, box1.orientation[1]), BMath::Vector3::Dot(dirVec, box1.orientation[2]));
		// Compute common subexpressions. Add in an epsilon term to
		// counteract arithmetic errors when two edges are parallel and
		// their cross product is (near) null (see text for details)
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				absRotationMatrix.data[i][j] = std::abs(rotationMatrix.data[i][j]) + EPSILON;
			}
		}

		// Check if any vertex of box1 is beyond the extents of box2 along box1's axes
		for (int i = 0; i < 3; i++) 
		{
			rBox1 = box1.extents[i];
			rBox2 = box2.extents[0] * absRotationMatrix.data[i][0] + box2.extents[1] * absRotationMatrix.data[i][1] + box2.extents[2] * absRotationMatrix.data[i][2];
			if (std::abs(dirVec[i]) > rBox1 + rBox2)
			{
				return false;
			}
		}

		// Check if any vertex of box2 is beyond the extents of box1 along box2's axes
		for (int i = 0; i < 3; i++) 
		{
			rBox1 = box1.extents[0] * absRotationMatrix.data[0][i] + box1.extents[1] * absRotationMatrix.data[1][i] + box1.extents[2] * absRotationMatrix.data[2][i];
			rBox2 = box2.extents[i];

			if (std::abs(dirVec[0] * rotationMatrix.data[0][i] + dirVec[1] * rotationMatrix.data[1][i] + dirVec[2] * rotationMatrix.data[2][i]) > rBox1 + rBox2)
			{
				return false;
			}
		}

		// Following tests are checking if there is an edge - edge collision

		rBox1 = box1.extents[1] * absRotationMatrix.data[2][0] + box1.extents[2] * absRotationMatrix.data[1][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[0][2] + box2.extents[2] * absRotationMatrix.data[0][1];
		if (std::abs(dirVec[2] * rotationMatrix.data[1][0] - dirVec[1] * rotationMatrix.data[2][0]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[1] * absRotationMatrix.data[2][1] + box1.extents[2] * absRotationMatrix.data[1][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[0][2] + box2.extents[2] * absRotationMatrix.data[0][0];
		if (std::abs(dirVec[2] * rotationMatrix.data[1][1] - dirVec[1] * rotationMatrix.data[2][1]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[1] * absRotationMatrix.data[2][2] + box1.extents[2] * absRotationMatrix.data[1][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[0][1] + box2.extents[1] * absRotationMatrix.data[0][0];
		if (std::abs(dirVec[2] * rotationMatrix.data[1][2] - dirVec[1] * rotationMatrix.data[2][2]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][0] + box1.extents[2] * absRotationMatrix.data[0][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[1][2] + box2.extents[2] * absRotationMatrix.data[1][1];
		if (std::abs(dirVec[0] * rotationMatrix.data[2][0] - dirVec[2] * rotationMatrix.data[0][0]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][1] + box1.extents[2] * absRotationMatrix.data[0][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[1][2] + box2.extents[2] * absRotationMatrix.data[1][0];
		if (std::abs(dirVec[0] * rotationMatrix.data[2][1] - dirVec[2] * rotationMatrix.data[0][1]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[2][2] + box1.extents[2] * absRotationMatrix.data[0][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[1][1] + box2.extents[1] * absRotationMatrix.data[1][0];
		if (std::abs(dirVec[0] * rotationMatrix.data[2][2] - dirVec[2] * rotationMatrix.data[0][2]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][0] + box1.extents[1] * absRotationMatrix.data[0][0];
		rBox2 = box2.extents[1] * absRotationMatrix.data[2][2] + box2.extents[2] * absRotationMatrix.data[2][1];
		if (std::abs(dirVec[1] * rotationMatrix.data[0][0] - dirVec[0] * rotationMatrix.data[1][0]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][1] + box1.extents[1] * absRotationMatrix.data[0][1];
		rBox2 = box2.extents[0] * absRotationMatrix.data[2][2] + box2.extents[2] * absRotationMatrix.data[2][0];
		if (std::abs(dirVec[1] * rotationMatrix.data[0][1] - dirVec[0] * rotationMatrix.data[1][1]) > rBox1 + rBox2)
		{
			return false;
		}

		rBox1 = box1.extents[0] * absRotationMatrix.data[1][2] + box1.extents[1] * absRotationMatrix.data[0][2];
		rBox2 = box2.extents[0] * absRotationMatrix.data[2][1] + box2.extents[1] * absRotationMatrix.data[2][0];
		if (std::abs(dirVec[1] * rotationMatrix.data[0][2] - dirVec[0] * rotationMatrix.data[1][2]) > rBox1 + rBox2)
		{
			return false;
		}

		// Since no separating axis is found, the OBBs must be intersecting
		return true;
	}

	bool BroadOBBPlaneCollision(const OBBCollider& box, const Plane& p)
	{
		float radius = box.extents[0] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[0])) +
					   box.extents[1] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[1])) +
					   box.extents[2] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[2]));

		float distance = BMath::Vector3::Dot(p.normal, box.position);
		return std::abs(distance) <= radius;
	}

	bool RayIntersectsAABB(const Ray& ray, const AABBCollider& aabb)
	{
		BMath::Vector3 aabbMin = aabb.position - aabb.extents;
		BMath::Vector3 aabbMax = aabb.position + aabb.extents;

		float nearestIntersectionDist = 0.0f;
		float farthestIntersectionDist = std::numeric_limits<float>::max();

		for (int axis = 0; axis < 3; axis++)
		{
			// Check for ray parallel to AABB's planes
			if (std::abs(ray.direction[axis]) < EPSILON)
			{
				// Ray is parallel and outside of AABB
				if (ray.origin[axis] < aabbMin[axis] || ray.origin[axis] > aabbMax[axis])
				{
					return false;
				}
			}
			else
			{
				float rayInverseDirection = 1.0f / ray.direction[axis];
				float intersectionDistMin = (aabbMin[axis] - ray.origin[axis]) * rayInverseDirection;
				float intersectionDistMax = (aabbMax[axis] - ray.origin[axis]) * rayInverseDirection;

				if (intersectionDistMin > intersectionDistMax)
				{
					std::swap(intersectionDistMin, intersectionDistMax);
				}

				if (intersectionDistMin > nearestIntersectionDist)
				{
					nearestIntersectionDist = intersectionDistMin;
				}

				if (intersectionDistMax < farthestIntersectionDist)
				{
					farthestIntersectionDist = intersectionDistMax;
				}

				if (nearestIntersectionDist > farthestIntersectionDist)
				{
					return false;
				}
			}
		}

		return true;
	}


	bool BroadRayAABBIntersection(const Ray& ray, const AABBCollider& collider)
	{
		const BMath::Vector3 min = collider.position - collider.extents;
		const BMath::Vector3 max = collider.position + collider.extents;

		const BMath::Vector3 invDir = BMath::Vector3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);

		// Calculate intersection with the slabs
		float t1 = (min.x - ray.origin.x) * invDir.x;
		float t2 = (max.x - ray.origin.x) * invDir.x;
		float t3 = (min.y - ray.origin.y) * invDir.y;
		float t4 = (max.y - ray.origin.y) * invDir.y;
		float t5 = (min.z - ray.origin.z) * invDir.z;
		float t6 = (max.z - ray.origin.z) * invDir.z;

		// Find the largest tMin and smallest tMax
		float tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tMax < 0 || tMin > tMax)
		{
			return false;
		}

		return true;
	}

	bool BroadRaySphereIntersection(const Ray& ray, const SphereCollider& sphere)
	{
		BMath::Vector3 rayToSphere = ray.origin - sphere.center;
		float sqDistToSphere = BMath::Vector3::Dot(rayToSphere, rayToSphere) - sphere.radius * sphere.radius;

		if (sqDistToSphere <= 0.0f)
		{
			return true;
		}

		float raySphereDirectionDot = BMath::Vector3::Dot(rayToSphere, ray.direction);
		if (raySphereDirectionDot > 0.0f)
		{
			return false;
		}

		float discriminant = raySphereDirectionDot * raySphereDirectionDot - sqDistToSphere;
		if (discriminant < 0.0f)
		{
			return false;
		}

		return true;
	}

	Point ClosestPBetweenPointAABB(const Point p, const AABBCollider& collider)
	{
		Point result{};
		BMath::Vector3 min = collider.position - collider.extents;
		BMath::Vector3 max = collider.position + collider.extents;

		for (int i = 0; i < 3; i++)
		{
			// Could be done with two ternary operators but less readable
			float temp = p[i];
			if (temp < min[i])
			{
				temp = min[i];
			}
			if (temp > max[i])
			{
				temp = max[i];
			}
			result[i] = temp;
		}

		return result;
	}
	float GetSqDistBetweenPointAABB(const Point p, const AABBCollider & collider)
	{
		float squaredDist = 0.0f;
		BMath::Vector3 min = collider.position - collider.extents;
		BMath::Vector3 max = collider.position + collider.extents;

		for (int i = 0; i < 3; i++)
		{
			float temp = p[i];
			if (temp < min[i])
			{
				squaredDist += (min[i] - temp) * (min[i] - temp);
			}
			if (temp > max[i])
			{
				squaredDist += (temp - max[i]) * (temp - max[i]);
			}
		}

		return squaredDist;
	}
}