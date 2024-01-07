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
	};
}
