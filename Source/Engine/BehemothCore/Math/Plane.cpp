#include "Plane.h"

namespace Math
{
	Plane Plane::TransformPlane(const Plane& plane, const Matrix4x4 matrix)
	{
		Vector4 extendedNormal(plane.normal.x, plane.normal.y, plane.normal.z, 0.0f);
		Vector4 transformedNormal = matrix * extendedNormal;
		Vector3 n = Vector3(transformedNormal.x, transformedNormal.y, transformedNormal.z).Normalize();

		Vector3 pointOnPlane = plane.normal * plane.distance;
		Vector4 extendedPointOnPlane = Vector4(pointOnPlane.x, pointOnPlane.y, pointOnPlane.z, 1.0f);
		Vector4 transformedPoint = matrix * extendedPointOnPlane;

		Plane transformedPlane{};
		transformedPlane.normal = n;
		transformedPlane.distance = Vector3::Dot(n, Vector3(transformedPoint.x, transformedPoint.y, transformedPoint.z));

		return transformedPlane;
	}
}