#include "pch.h"
#include "LevelHelper.h"
#include "Components/Components.h"
#include "Components/PhysicsComponents.h"
#include "GameComponents/Player/PlayerComponent.h"

void LevelHelper::CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle)
{
	if (Behemoth::TriggerDataComponent* triggerData = registry.GetComponent<Behemoth::TriggerDataComponent>(playerHandle))
	{
		PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
		if (!playerComponent)
		{
			LOGMESSAGE(Error, "Null player component found");
			return;
		}

		if (playerComponent->isRespawning)
		{
			// Player fell is already being processed
			return;
		}

		for (const auto& d : triggerData->data)
		{
			if (d.otherHandle.ID == oobHandle.ID)
			{

				registry.AddComponent<PlayerFellComponent>(playerHandle, 3.0f);
			}
		}
	}
}
