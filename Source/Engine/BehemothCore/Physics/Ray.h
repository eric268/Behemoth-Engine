#pragma once
#include "Math/MathCore.h"
#include "Collision/CollisionMask.h"

namespace Behemoth
{
	class Ray
	{
	public:
		Ray();
		Ray(BMath::Vector3 position, BMath::Vector3 direction);

		BMath::Vector3 origin;
		BMath::Vector3 direction;
	};

}