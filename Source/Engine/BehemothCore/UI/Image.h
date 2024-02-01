#pragma once

#include "Math/Vector.h"

#include <string>
#include <memory>

class CSimpleSprite;

namespace Behemoth
{
	class Image
	{
	public:
		Image();
		Image(std::string path, BMath::Vector2 d);

		void Draw(const BMath::Vector2& position);

		inline BMath::Vector2& GetDimensions() { return dimension; }
		inline void SetDimensions(BMath::Vector2 d) { dimension = d; }

	private:
		std::shared_ptr<CSimpleSprite> imageSprite;
		BMath::Vector2 dimension;
		std::string imageFilepath;
	};
}