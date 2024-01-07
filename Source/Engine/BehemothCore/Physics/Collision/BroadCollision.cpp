#include "pch.h"
#include "BroadCollision.h"
#include "Math/MathCore.h"
#include "Colliders.h"
#include "ECS/ECSCore.h"
#include "Physics/Collision/Colliders.h"
#include "Geometry/Plane.h"

namespace Behemoth
{
	bool BroadAABBCollision(const AABBCollider& box1, const AABBCollider& box2)
	{
		BMath::Vector3 minPos1 = box1.position - box1.extents;
		BMath::Vector3 maxPos1 = box1.position + box1.extents;

		BMath::Vector3 minPos2 = box2.position - box2.extents;
		BMath::Vector3 maxPos2 = box2.position + box2.extents;


		if (maxPos1.x < minPos2.x || minPos1.x > maxPos2.x)
			return false;
		if (maxPos1.y < minPos2.y || minPos1.y > maxPos2.y)
			return false;
		if (maxPos1.z < minPos2.z || minPos1.z > maxPos2.z)
			return false;

		return true;
	}
	bool BroadAABBPlaneCollision(const AABBCollider& collider, const Plane& p)
	{
		// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
		float radius = collider.extents[0] * std::abs(p.normal[0]) + collider.extents[1] * std::abs(p.normal[1]) + collider.extents[2] * std::abs(p.normal[2]);

		float distance = BMath::Vector3::Dot(p.normal, collider.position) - p.distance;

		// Intersection occurs when distance s falls within [-r,+r] interval
		return std::abs(distance) <= radius;
	}

	bool BroadSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2)
	{
		float distance = BMath::Vector3::SquaredDistance(sphere1.positionOffset, sphere2.positionOffset);
		float rad = sphere1.radius + sphere2.radius;
		return distance <= (rad * rad);
	}
	bool BroadSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box)
	{
		float squaredDist = 0;
		float squaredRad = sphere.radius * sphere.radius;

		for (int axis = 0; axis < 3; axis++)
		{
			float diff = 0;
			float minVal = box.position[axis] - box.extents[axis];
			float maxVal = box.position[axis] + box.extents[axis];

			float spherePos = sphere.positionOffset[axis];

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

	// Real-Time Collision Detection-Morgan Kaufmann (2005)
	bool BroadOBBCollision(const OBBCollider& box1, const OBBCollider& box2)
	{
		float rBox1, rBox2;

		BMath::Matrix3x3 R, AbsR;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				R.data[i][j] = BMath::Vector3::Dot(box1.orientation[i], box2.orientation[j]);
			}
		}

		BMath::Vector3 t = box2.pos - box1.pos;
		t = BMath::Vector3(BMath::Vector3::Dot(t, box1.orientation[0]), BMath::Vector3::Dot(t, box1.orientation[1]), BMath::Vector3::Dot(t, box1.orientation[2]));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				AbsR.data[i][j] = std::abs(R.data[i][j]) + EPSILON;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.halfwidthExtents[i];
			rBox2 = box2.halfwidthExtents[0] * AbsR.data[i][0] + box2.halfwidthExtents[1] * AbsR.data[i][1] + box2.halfwidthExtents[2] * AbsR.data[i][2];
			if (std::abs(t[i]) > (rBox1 + rBox2))
			{
				return false;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.halfwidthExtents[0] * AbsR.data[0][i] + box1.halfwidthExtents[1] * AbsR.data[1][i] + box1.halfwidthExtents[2] * AbsR.data[2][i];
			rBox2 = box2.halfwidthExtents[i];
			if (std::abs(t[0] * R.data[0][i] + t[1] * R.data[1][i] + t[2] * R.data[2][i]) > (rBox1 + rBox2))
			{
				return false;
			}
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][0] + box1.halfwidthExtents[2] * AbsR.data[1][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[0][2] + box2.halfwidthExtents[2] * AbsR.data[0][1];
		if (std::abs(t[2] + R.data[1][0] - t[1] * R.data[2][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][1] + box1.halfwidthExtents[2] * AbsR.data[1][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[0][2] + box2.halfwidthExtents[2] * AbsR.data[0][0];
		if (std::abs(t[2] + R.data[1][1] - t[1] * R.data[2][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][2] + box1.halfwidthExtents[2] * AbsR.data[1][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[0][1] + box2.halfwidthExtents[1] * AbsR.data[0][0];
		if (std::abs(t[2] + R.data[1][2] - t[1] * R.data[2][2] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][0] + box1.halfwidthExtents[2] * AbsR.data[0][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[1][2] + box2.halfwidthExtents[2] * AbsR.data[1][1];
		if (std::abs(t[0] + R.data[2][0] - t[2] * R.data[0][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][1] + box1.halfwidthExtents[2] * AbsR.data[0][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[1][2] + box2.halfwidthExtents[2] * AbsR.data[1][0];
		if (std::abs(t[0] + R.data[2][1] - t[2] * R.data[0][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][2] + box1.halfwidthExtents[2] * AbsR.data[0][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[1][1] + box2.halfwidthExtents[1] * AbsR.data[1][0];
		if (std::abs(t[0] + R.data[2][2] - t[2] * R.data[0][2] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][0] + box1.halfwidthExtents[1] * AbsR.data[0][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[2][2] + box2.halfwidthExtents[2] * AbsR.data[2][1];
		if (std::abs(t[1] + R.data[0][0] - t[0] * R.data[1][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][1] + box1.halfwidthExtents[1] * AbsR.data[0][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[2][2] + box2.halfwidthExtents[2] * AbsR.data[2][0];
		if (std::abs(t[1] + R.data[0][1] - t[0] * R.data[1][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][2] + box1.halfwidthExtents[1] * AbsR.data[0][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[2][1] + box2.halfwidthExtents[1] * AbsR.data[2][0];
		if (std::abs(t[1] + R.data[0][2] - t[0] * R.data[1][2] > rBox1 + rBox2))
		{
			return false;
		}

		return true;
	}
	bool BroadOBBPlaneCollision(const OBBCollider& box, const Plane& p)
	{
		float radius = box.halfwidthExtents[0] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[0])) +
					   box.halfwidthExtents[1] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[1])) +
					   box.halfwidthExtents[2] * std::abs(BMath::Vector3::Dot(p.normal, box.orientation[2]));

		float distance = BMath::Vector3::Dot(p.normal, box.pos);
		return std::abs(distance) <= radius;
	}

	bool BroadLinePlaneIntersection(const Point& p1, const Point& p2, const Plane& plane, float& dist, Point& intersectionP)
	{
		BMath::Vector3 ab = p2 - p1;
		dist = (plane.distance - BMath::Vector3::Dot(plane.normal, p1)) / (BMath::Vector3::Dot(plane.normal, ab));
		if (dist >= 0.0f && dist <= 1.0f)
		{
			intersectionP = p1 + ab * dist;
			return true;
		}

		return false;
	}
	bool BroadLineAABBIntersection(const Point& lineStart, const Point& lineEnd, const AABBCollider& box)
	{
		BMath::Vector3 boxMin = box.position - box.extents;  // Minimum corner of the AABB
		BMath::Vector3 boxMax = box.position + box.extents;  // Maximum corner of the AABB

		Point boxCenter = (boxMin + boxMax) * 0.5f;  // Center point of the AABB
		BMath::Vector3 boxExtentsHalf = boxMax - boxCenter; // Half extents of the AABB
		Point lineMidpoint = (lineStart + lineEnd) * 0.5f;  // Midpoint of the line segment
		BMath::Vector3 lineHalfVector = lineEnd - lineMidpoint;  // Vector from midpoint to end of the line segment
		lineMidpoint = lineMidpoint - boxCenter;  // Translate line midpoint into box's local space

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

	// Primitive Collision
	Point GetClosestPointOnPlane(Point point, const Plane& plane)
	{
		float t = BMath::Vector3::Dot(plane.normal, point);
		return point - plane.normal * t;
	}
	float GetDistBetweenPointPlane(const Point point, const Plane& plane)
	{
		return BMath::Vector3::Dot(plane.normal, point) - plane.distance;
	}

	bool BroadRayAABBIntersection(const Ray& ray, const AABBCollider& collider, float& minDist, Point& collisionPoint)
	{
		BMath::Vector3 min = collider.position - collider.extents;
		BMath::Vector3 max = collider.position + collider.extents;

		minDist = 0.0f;
		float maxDist = std::numeric_limits<float>::max();

		for (int i = 0; i < 3; i++)
		{
			// If ray is parallel then check if the origin of the ray is not inside the box
			if (std::abs(ray.direction[i]) < EPSILON)
			{
				if (ray.origin[i] < min[i] || ray.origin[i] > max[i])
				{
					return false;
				}
			}

			float inverseDirection = 1.0f / ray.direction[i];
			float t0 = (min[i] - ray.origin[i]) * inverseDirection;
			float t1 = (max[i] - ray.origin[i]) * inverseDirection;

			if (inverseDirection < 0.0f)
			{
				std::swap(t0, t1);
			}

			minDist = std::max(minDist, t0);
			maxDist = std::max(maxDist, t1);

			if (minDist < maxDist)
			{
				return false;
			}
		}

		collisionPoint = ray.origin + ray.direction * minDist;
		return true;
	}
	bool BroadRaySphereIntersection(const Ray& ray, const SphereCollider& sphere)
	{
		BMath::Vector3 rayToSphere = ray.origin - sphere.positionOffset;
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