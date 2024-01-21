#include "pch.h"
#include "PlayerHUDSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/UIComponents.h"

using namespace Behemoth;

void PlayerHUDSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [entity, playerComponent, playerHUD] : registry.Get<PlayerComponent, PlayerHUDComponent>())
	{
		TextComponent* powerUIText = registry.GetComponent<TextComponent>(playerHUD->powerUIHandle);
		if (powerUIText)
		{
			powerUIText->text = "Power: " + std::to_string((int)playerComponent->currentPower);
		}

		TextComponent* strokesUIText = registry.GetComponent<TextComponent>(playerHUD->strokesUIHandle);
		if (strokesUIText)
		{
			strokesUIText->text = "Strokes: " + std::to_string(playerComponent->strokesUsed);
		}
	}
}
