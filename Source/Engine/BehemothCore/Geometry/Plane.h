#pragma once
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace Behemoth
{
	class Plane
	{
	public:
		Plane();
		Plane(BMath::Vector3 n, float d) : normal(n), d(d) {}
		BMath::Vector3 normal;
		float d;


		static Plane TransformPlane(const Plane& plane, const BMath::BMatrix4x4 m);
		static inline BMath::Vector3 GetPointOnPlane(const Plane& plane) { return plane.normal * plane.d; }
		static inline float CalculatePlaneOffset(const BMath::Vector3& normal, const BMath::Vector3& point) 
		{ 
			return -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
		}

		// Point Plane
		static BMath::Vector3 GetClosestPointOnPlane(const BMath::Vector3 point, const Plane& plane);
		static float GetDistBetweenPointPlane(const BMath::Vector3 point, const Plane& plane);
		static float DistToOrigin(const BMath::Vector3 point, const Plane& plane);
	};
}
