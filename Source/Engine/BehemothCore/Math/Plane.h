#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace Math
{
	class Plane
	{
	public:
		Plane() = default;
		Plane(Vector3 n, float d) : normal(n), distance(d) {}
		Vector3 normal;
		float distance;

		static Plane TransformPlane(const Plane& plane, const Matrix4x4 m);
	};
}
