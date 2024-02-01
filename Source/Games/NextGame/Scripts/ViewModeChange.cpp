#include "pch.h"
#include "ViewModeChange.h"
#include "Components/Components.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/goalViewComponent.h"

bool ViewMode::ToggleViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle goalViewHandle)
{
	GoalViewComponent* goalViewComp = registry.GetComponent<GoalViewComponent>(goalViewHandle);
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(playerHandle);

	if (!goalViewComp || !playerComp)
	{
		LOGMESSAGE(Error, "Null component found while changing view mode");
		return false;
	}

	if (Behemoth::CameraComponent* goalViewCameraComp = registry.GetComponent<Behemoth::CameraComponent>(goalViewComp->attachedCamera))
	{
		goalViewCameraComp->isMain = !goalViewCameraComp->isMain;
	}
	else
	{
		LOGMESSAGE(Error, "Unable to find level view camera");
		return false;
	}
	goalViewComp->isActive = !goalViewComp->isActive;

	if (Behemoth::CameraComponent* playerCameraComp = registry.GetComponent<Behemoth::CameraComponent>(playerComp->attachedCamera))
	{
		playerCameraComp->isMain = !playerCameraComp->isMain;
	}
	else
	{
		LOGMESSAGE(Error, "Unable to find level player camera");
		return false;
	}
	playerComp->isActive = !playerComp->isActive;
	
    return true;
}
