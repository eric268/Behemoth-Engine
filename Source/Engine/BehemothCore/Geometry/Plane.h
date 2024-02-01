#pragma once

#include "Math/MathCore.h"

namespace Behemoth
{
	struct Plane
	{
		Plane();
		Plane(BMath::Vector3 n, float d);

		static inline BMath::Vector3 GetPointOnPlane(const Plane& plane) { return plane.normal * plane.d; }
		static inline float CalculatePlaneOffset(const BMath::Vector3& normal, const BMath::Vector3& point) 
		{ 
			return -(normal.x * point.x + normal.y * point.y + normal.z * point.z);
		}

		static BMath::Vector3 GetClosestPointOnPlane(const BMath::Vector3 point, const Plane& plane);
		static float GetDistBetweenPointPlane(const BMath::Vector3 point, const Plane& plane);

		BMath::Vector3 normal;
		float d;
	};
}
