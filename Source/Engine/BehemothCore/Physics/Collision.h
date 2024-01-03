#pragma once

#include "Math/MathCore.h"

namespace Behemoth
{
	bool CheckAABBCollision(const Math::Vector3& box1Min, const Math::Vector3& box1Max, const Math::Vector3& box2Min, const Math::Vector3& box2Max);
	bool CheckSphereSphereCollision(const Math::Vector3 sphere1Pos, const float sphere1Radius, const Math::Vector3 sphere2Pos, const float sphere2Radius);
}

