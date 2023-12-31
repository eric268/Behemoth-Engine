#pragma once
#include "Math/Vector.h"
namespace Behemoth
{
	struct Line
	{
		Line() = default;
		Line(Math::Vector4 data, Math::Vector3 color) : lineData(data), lineColor(color) {}
		Math::Vector4 lineData;
		Math::Vector3 lineColor;
	};
}