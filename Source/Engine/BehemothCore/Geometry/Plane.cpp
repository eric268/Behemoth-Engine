#include "pch.h"
#include "Plane.h"

namespace Behemoth
{
	Plane::Plane() : normal(BMath::Vector3()), d(0.0f) {}

	Plane::Plane(BMath::Vector3 n, float d) : normal(n), d(d) {}

	BMath::Vector3 Plane::GetClosestPointOnPlane(BMath::Vector3 point, const Plane& plane)
	{
		float t = BMath::Vector3::Dot(plane.normal, point);
		return point - plane.normal * t;
	}
	float Plane::GetDistBetweenPointPlane(const BMath::Vector3 point, const Plane& plane)
	{
		return BMath::Vector3::Dot(plane.normal, point) - plane.d;
	}
}