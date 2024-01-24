#include "pch.h"
#include "ProjectileMotion.h"

namespace Behemoth
{
	BMath::Vector3 ProjectileMotion::CalculateInitalVelocity(float power, const BMath::Vector3& direction)
	{
		BMath::Vector3 dir = direction;
		dir.Normalize();

		return dir * power;
	}
}