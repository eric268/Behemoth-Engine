#pragma once
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace Behemoth
{
	class Plane
	{
	public:
		Plane();
		Plane(Math::Vector3 n, float d) : normal(n), distance(d) {}
		Math::Vector3 normal;
		float distance;

		static Plane TransformPlane(const Plane& plane, const Math::Matrix4x4 m);
	};
}
