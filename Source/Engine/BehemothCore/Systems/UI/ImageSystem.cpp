#include "pch.h"
#include "ImageSystem.h"
#include "ECS/Registry.h"
#include "Components/UIComponents.h"

namespace Behemoth
{
	void ImageSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, imageComponent] : registry.Get<ImageComponent>())
		{
			if (!imageComponent->isVisible)
			{
				continue;
			}

			imageComponent->image.Draw(imageComponent->screenPosition);
		}
	}
}