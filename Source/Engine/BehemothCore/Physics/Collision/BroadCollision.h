#pragma once
#include "Components/PhysicsComponents.h"
#include "Geometry/Plane.h"
#include "Physics/Ray.h"

namespace Behemoth
{
	class Plane;

	using Point = Math::Vector3;

	bool BroadAABBCollision(const AABBCollider& box1, const AABBCollider& box2);
	bool BroadAABBPlaneCollision(const AABBCollider& box, const Plane& p);

	bool BroadSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2);
	bool BroadSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box);

	bool BroadOBBCollision(const OBBCollider& box1, const OBBCollider& box2);
	bool BroadOBBPlaneCollision(const OBBCollider& box, const Plane& p);

	// Line segements
	bool BroadLinePlaneIntersection(const Point& p1, const Point& p2, const Plane& plane, float& dist, Point& intersectionP);
	bool BroadLineAABBIntersection(const Point& p1, const Point& p2, const AABBCollider& box);

	// Raycasts
	bool BroadRayAABBIntersection(const Ray& ray, const AABBCollider& collider, float& minDist, Point& collisionPoint);
	bool BroadRaySphereIntersection(const Ray& ray, const SphereCollider& sphere);


	// Point Plane
	Point GetClosestPointOnPlane(const Point point, const Plane& plane);
	float GetDistBetweenPointPlane(const Point point, const Plane& plane);

	// Point AABB
	Point ClosestPBetweenPointAABB(const Point p, const AABBCollider& collider);
	float GetSqDistBetweenPointAABB(const Point p, const AABBCollider& collider);
}