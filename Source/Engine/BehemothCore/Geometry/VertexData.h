#pragma once
#include "Math/Vector.h"

namespace Behemoth
{
	struct VertexData
	{
		BMath::Vector3 vertex;
		BMath::Vector3 normal;
		BMath::Vector2 uv;
	};
}