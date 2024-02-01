#include "pch.h"
#include "TextSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/UIComponents.h"


namespace Behemoth
{
	void TextSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entityHandle, textComponent] : registry.Get<TextComponent>())
		{
			textComponent->text.Print(textComponent->screenPosition);
		}
	}
}