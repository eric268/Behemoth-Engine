#pragma once
#include "Math/Vector.h"

namespace Behemoth
{
	enum BColor
	{
		Red,
		Green,
		Blue,
		Magenta,
		Yellow,
		Cyan,
		White
	};

	static BMath::Vector3 colors[7] =
	{
		BMath::Vector3(1.0f, 0.0f, 0.0f),
		BMath::Vector3(0.0f, 1.0f, 0.0f),
		BMath::Vector3(0.0f, 0.0f, 1.0f),
		BMath::Vector3(1.0f, 0.0f, 1.0f),
		BMath::Vector3(1.0f, 1.0f, 0.0f),
		BMath::Vector3(0.0f, 1.0f, 1.0f),
		BMath::Vector3(1.0f, 1.0f, 1.0f)
	};

	static BMath::Vector3 GetColor(BColor color)
	{
		return colors[color];
	}
}