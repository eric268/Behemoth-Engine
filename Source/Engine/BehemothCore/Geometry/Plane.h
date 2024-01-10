#pragma once
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace Behemoth
{
	class Plane
	{
	public:
		Plane();
		Plane(BMath::Vector3 n, float d) : normal(n), distance(d) {}
		BMath::Vector3 normal;
		float distance;

		static Plane TransformPlane(const Plane& plane, const BMath::Matrix4x4 m);
		static inline BMath::Vector3 GetPointOnPlane(const Plane& plane) { return plane.normal * plane.distance; }
		static inline float CalculatePlaneOffset(const BMath::Vector3& normal, const BMath::Vector3& point) 
		{ 
			return -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
		}
	};
}
