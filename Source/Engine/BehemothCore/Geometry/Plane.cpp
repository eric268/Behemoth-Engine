#include "pch.h"
#include "Plane.h"

namespace Behemoth
{
	Plane::Plane() : normal(BMath::Vector3()), distance(0.0f) {}

	Plane Plane::TransformPlane(const Plane& plane, const BMath::Matrix4x4 matrix)
	{
		BMath::Vector4 transformedNormal = matrix * BMath::Vector4(plane.normal.x, plane.normal.y, plane.normal.z, 0.0f);
		BMath::Vector3 normal = BMath::Vector3(transformedNormal.x, transformedNormal.y, transformedNormal.z).Normalize();

		BMath::Vector3 pointOnPlane = plane.normal * plane.distance;
		BMath::Vector4 transformedPoint = matrix * BMath::Vector4(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z, 1.0f);

		Plane transformedPlane{};
		transformedPlane.normal = normal;
		transformedPlane.distance = BMath::Vector3::Dot(normal, BMath::Vector3(transformedPoint.x, transformedPoint.y, transformedPoint.z));

		return transformedPlane;
	}
}