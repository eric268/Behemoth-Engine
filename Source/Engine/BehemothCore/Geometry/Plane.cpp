#include "pch.h"
#include "Plane.h"

namespace Behemoth
{
	Plane::Plane() : normal(BMath::Vector3()), d(0.0f) {}

	Plane Plane::TransformPlane(const Plane& plane, const BMath::Matrix4x4 matrix)
	{
		BMath::Vector4 transformedNormal = matrix * BMath::Vector4(plane.normal.x, plane.normal.y, plane.normal.z, 0.0f);
		BMath::Vector3 normal = BMath::Vector3(transformedNormal.x, transformedNormal.y, transformedNormal.z).Normalize();

		BMath::Vector3 pointOnPlane = plane.normal * plane.d;
		BMath::Vector4 transformedPoint = matrix * BMath::Vector4(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z, 1.0f);

		Plane transformedPlane{};
		transformedPlane.normal = normal;
		transformedPlane.d = BMath::Vector3::Dot(normal, BMath::Vector3(transformedPoint.x, transformedPoint.y, transformedPoint.z));

		return transformedPlane;
	}

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