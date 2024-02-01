#pragma once

#include "Math/Vector.h"

namespace Behemoth
{
	struct VertexData
	{
		BMath::Vector3 position;
		BMath::Vector3 normal;
		BMath::Vector2 uv;
	};
}