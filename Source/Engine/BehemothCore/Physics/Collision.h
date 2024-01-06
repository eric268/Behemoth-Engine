#pragma once
#include "Components/PhysicsComponents.h"
#include "Math/Plane.h"
#include "Ray.h"

namespace Behemoth::Collision
{
	using Point = Math::Vector3;

	bool CheckAABBCollision(const AABBCollider& box1, const AABBCollider& box2);
	bool CheckAABBPlaneCollision(const AABBCollider& box, const Math::Plane& p);

	bool CheckSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2);
	bool CheckSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box);

	bool CheckOBBCollision(const OBBCollider& box1, const OBBCollider& box2);
	bool CheckOBBPlaneCollision(const OBBCollider& box, const Math::Plane& p);

	// Line segements
	bool CheckLinePlaneIntersection(const Point& p1, const Point& p2, const Math::Plane& plane, float& dist, Point& intersectionP);
	bool CheckLineAABBIntersection(const Point& p1, const Point& p2, const AABBCollider& box);

	// Raycasts
	bool RayAABBIsColliding(const Ray& ray, const AABBCollider& collider, float& minDist, Point& collisionPoint);
	bool RaySphereIsColliding(const Ray& ray, const SphereCollider& sphere);


	// Point Plane
	Point GetClosestPointOnPlane(const Point point, const Math::Plane& plane);
	float GetDistBetweenPointPlane(const Point point, const Math::Plane& plane);

	// Point AABB
	Point ClosestPBetweenPointAABB(const Point p, const AABBCollider& collider);
	float GetSqDistBetweenPointAABB(const Point p, const AABBCollider& collider);
}