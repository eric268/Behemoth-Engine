#pragma once

#include "Components/PhysicsComponents.h"

// Class that contains broad collision detection functions
// Majority of functions do not generate any contact data
namespace Behemoth
{
	class Ray;
	class Plane;
	struct CollisionData;

	using Point = BMath::Vector3;

	// AABB
	bool BroadAABBCollision(const AABBCollider& box1, const AABBCollider& box2);
	bool BroadAABBPlaneCollision(const AABBCollider& box, const Plane& p);

	// Sphere
	bool BroadSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2);
	bool BroadSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box);

	// OBB
	bool BroadOBBCollision(const OBBCollider& box1, const OBBCollider& box2);
	bool BroadOBBPlaneCollision(const OBBCollider& box, const Plane& p);

	// Line segments
	bool BroadLinePlaneIntersection(const Point& p1, const Point& p2, const Plane& plane, float& dist, Point& intersectionP);
	bool BroadLineAABBIntersection(const Point& p1, const Point& p2, const AABBCollider& box);

	// Ray casts
	bool BroadRayAABBIntersection(const Ray& ray, const AABBCollider& collider);
	bool BroadRaySphereIntersection(const Ray& ray, const SphereCollider& sphere);

	// Point AABB
	Point ClosestPBetweenPointAABB(const Point p, const AABBCollider& collider);
	float GetSqDistBetweenPointAABB(const Point p, const AABBCollider& collider);
}