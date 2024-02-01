#include "pch.h"
#include "Image.h"
#include "NextAPI/App/SimpleSprite.h"
#include "Core/ResourceManager.h"

namespace Behemoth
{
	Image::Image() : imageSprite(nullptr), imageFilepath(""), dimension() {}

	Image::Image(std::string path, BMath::Vector2 d) : dimension(d)
	{
		const std::string& imagePath = ResourceManager::GetInstance().GetImagePath(path);
		imageSprite = std::make_shared<CSimpleSprite>(imagePath.c_str());

		float x = dimension.x;
		float y = dimension.y;
		imageSprite->SetUV(0, -x * 0.5f, -y * 0.5f);
		imageSprite->SetUV(1,  x * 0.5f, -y * 0.5f);
		imageSprite->SetUV(2,  x * 0.5f,  y * 0.5f);
		imageSprite->SetUV(3, -x * 0.5f,  y * 0.5f);
	}

	void Image::Draw(const BMath::Vector2& position)
	{
		// Bottom Left
		imageSprite->SetVertex(0, position.x - dimension.x, position.y - dimension.y);
		// Bottom Right
		imageSprite->SetVertex(1, position.x + dimension.x, position.y - dimension.y);
		// Top Right
		imageSprite->SetVertex(2, position.x + dimension.x, position.y + dimension.y);
		// Bottom Right
		imageSprite->SetVertex(3, position.x - dimension.x, position.y + dimension.y);

		if (imageSprite)
		{
			imageSprite->Draw();
		}
	}
}
