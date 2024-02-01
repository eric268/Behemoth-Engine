#include "pch.h"
#include "ViewModeChange.h"
#include "Components/Components.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/LevelViewComponent.h"

bool ViewMode::ToggleViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle levelViewHandle)
{
	LevelViewComponent* levelViewComp = registry.GetComponent<LevelViewComponent>(levelViewHandle);
	PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(playerHandle);

	if (!levelViewComp || !playerComp)
	{
		LOGMESSAGE(Error, "Null component found while changing view mode");
		return false;
	}

	if (Behemoth::CameraComponent* levelViewCameraComp = registry.GetComponent<Behemoth::CameraComponent>(levelViewComp->attachedCamera))
	{
		levelViewCameraComp->isMain = !levelViewCameraComp->isMain;
	}
	else
	{
		LOGMESSAGE(Error, "Unable to find level view camera");
		return false;
	}
	levelViewComp->isActive = !levelViewComp->isActive;

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
