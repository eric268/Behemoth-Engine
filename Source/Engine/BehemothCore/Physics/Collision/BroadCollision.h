#pragma once
#include "Components/PhysicsComponents.h"

namespace Behemoth
{
	class Ray;
	class Plane;
	struct CollisionData;

	using Point = BMath::Vector3;

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

	// Point AABB
	Point ClosestPBetweenPointAABB(const Point p, const AABBCollider& collider);
	float GetSqDistBetweenPointAABB(const Point p, const AABBCollider& collider);
}