#include "pch.h"
#include "Plane.h"

namespace Math
{
	Plane Plane::TransformPlane(const Plane& plane, const Matrix4x4 matrix)
	{
		Vector4 transformedNormal = matrix * Vector4(plane.normal.x, plane.normal.y, plane.normal.z, 0.0f);
		Vector3 normal = Vector3(transformedNormal.x, transformedNormal.y, transformedNormal.z).Normalize();

		Vector3 pointOnPlane = plane.normal * plane.distance;
		Vector4 transformedPoint = matrix * Vector4(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z, 1.0f);

		Plane transformedPlane{};
		transformedPlane.normal = normal;
		transformedPlane.distance = Vector3::Dot(normal, Vector3(transformedPoint.x, transformedPoint.y, transformedPoint.z));

		return transformedPlane;
	}
}