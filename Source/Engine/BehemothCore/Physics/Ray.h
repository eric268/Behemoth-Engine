#pragma once
#include "Math/MathCore.h"
#include "CollisionMask.h"

namespace Behemoth
{
	class Ray
	{
	public:
		Ray();
		Ray(Math::Vector3 position, Math::Vector3 direction);

		Math::Vector3 origin;
		Math::Vector3 direction;
	};

}