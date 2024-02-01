#pragma once

#include "Math/Vector.h"

namespace Behemoth
{
	struct Line
	{
		Line() = default;
		Line(BMath::Vector4 data, BMath::Vector3 color) : lineData(data), lineColor(color) {}
		BMath::Vector4 lineData;
		BMath::Vector3 lineColor;
	};
}