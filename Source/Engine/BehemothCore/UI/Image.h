#pragma once

#include "Math/Vector.h"

class CSimpleSprite;

namespace Behemoth
{
	class Image
	{
	public:

	private:
		CSimpleSprite* imageSprite;
		BMath::Vector2 position;
	};
}