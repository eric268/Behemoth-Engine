#pragma once

#include "Math/MathCore.h"

namespace Behemoth
{
	class ProjectileMotion
	{
	public:

		static BMath::Vector3 CalculateInitalVelocity(float power, const BMath::Vector3& direction);

	private:

	};
}