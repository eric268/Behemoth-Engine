#pragma once
#include "Math/MathCore.h"

namespace Behemoth
{
	struct OBB
	{
		Math::Vector3 point;
		Math::Vector3 rotVecs[3];
		Math::Vector3 halfwidthExtents;
	};
}