#pragma once

#include "Math/MathCore.h"

namespace Behemoth
{
	bool CheckAABBCollision(const Math::Vector3& box1Min, const Math::Vector3& box1Max, const Math::Vector3& box2Min, const Math::Vector3& box2Max);
}

