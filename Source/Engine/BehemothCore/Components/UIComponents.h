#pragma once
#include "ECS/Component.h"
#include "UI/Image.h"
#include "UI/Text.h"

#include <string>

namespace Behemoth
{
	struct TextComponent : ECS::Component
	{
		TextComponent() : text(""), screenPosition() {}
		TextComponent(std::string t, BMath::Vector2 position) : text(t), screenPosition(position) {}

		Text text;
		BMath::Vector2 screenPosition;
	};

	struct ImageComponent : ECS::Component 
	{
		ImageComponent() : image(), isVisible(false), screenPosition() {}

		ImageComponent(std::string filepath, BMath::Vector2 position, BMath::Vector2 dimensions, bool isVisible = true) : 
			image(filepath, dimensions),
			screenPosition(position),
			isVisible(isVisible)  {}

		Image image;
		bool isVisible;
		BMath::Vector2 screenPosition;
	};
}