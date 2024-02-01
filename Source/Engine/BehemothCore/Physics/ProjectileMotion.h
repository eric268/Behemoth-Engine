#pragma once

#include "Math/MathCore.h"

namespace Behemoth
{
	struct ProjectileMotion
	{
		static BMath::Vector3 CalculateInitalVelocity(const float power, const BMath::Vector3& direction);
	};
}