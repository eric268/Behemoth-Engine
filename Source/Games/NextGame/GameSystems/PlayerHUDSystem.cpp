#include "pch.h"
#include "PlayerHUDSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/UIComponents.h"

void PlayerHUDSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [entityHandle, playerComp, playerHUDComp] : registry.Get<PlayerComponent, PlayerHUDComponent>())
	{
		Behemoth::TextComponent* powerUITextComp = registry.GetComponent<Behemoth::TextComponent>(playerHUDComp->powerUIHandle);
		if (powerUITextComp)
		{
			powerUITextComp->text = "Power: " + std::to_string((int)playerComp->currentPower);
		}

		Behemoth::TextComponent* strokesUITextComp = registry.GetComponent<Behemoth::TextComponent>(playerHUDComp->strokesUIHandle);
		if (strokesUITextComp)
		{
			strokesUITextComp->text = "Strokes: " + std::to_string(playerComp->strokesUsed);
		}
	}
}
