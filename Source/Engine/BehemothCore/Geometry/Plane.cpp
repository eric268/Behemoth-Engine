#include "pch.h"
#include "Plane.h"

namespace Behemoth
{
	Plane::Plane() : normal(Math::Vector3()), distance(0.0f) {}

	Plane Plane::TransformPlane(const Plane& plane, const Math::Matrix4x4 matrix)
	{
		Math::Vector4 transformedNormal = matrix * Math::Vector4(plane.normal.x, plane.normal.y, plane.normal.z, 0.0f);
		Math::Vector3 normal = Math::Vector3(transformedNormal.x, transformedNormal.y, transformedNormal.z).Normalize();

		Math::Vector3 pointOnPlane = plane.normal * plane.distance;
		Math::Vector4 transformedPoint = matrix * Math::Vector4(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z, 1.0f);

		Plane transformedPlane{};
		transformedPlane.normal = normal;
		transformedPlane.distance = Math::Vector3::Dot(normal, Math::Vector3(transformedPoint.x, transformedPoint.y, transformedPoint.z));

		return transformedPlane;
	}
}