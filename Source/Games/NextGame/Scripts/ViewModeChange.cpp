#include "pch.h"
#include "ViewModeChange.h"
#include "Components/Components.h"

using namespace Behemoth;

bool ViewModeChange::ChangeViewMode(ECS::Registry& registry, ECS::EntityHandle playerHandle, ECS::EntityHandle levelViewHandle)
{
	LevelViewComponent* levelViewComponent = registry.GetComponent<LevelViewComponent>(levelViewHandle);
	PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);

	if (!levelViewComponent || !playerComponent)
	{
		LOGMESSAGE(Error, "Null component found while changing view mode");
		return false;
	}

	CameraComponent* camera = registry.GetComponent<CameraComponent>(levelViewComponent->attachedCamera);
	if (camera)
	{
		camera->isMain = !camera->isMain;
	}
	else
	{
		LOGMESSAGE(Error, "Unable to find level view camera");
		return false;
	}
	levelViewComponent->isActive = !levelViewComponent->isActive;

	CameraComponent* playerCamera = registry.GetComponent<CameraComponent>(playerComponent->attachedCamera);
	if (playerCamera)
	{
		playerCamera->isMain = !playerCamera->isMain;
	}
	else
	{
		LOGMESSAGE(Error, "Unable to find level player camera");
		return false;
	}
	playerComponent->isActive = !playerComponent->isActive;
	

    return true;
}
