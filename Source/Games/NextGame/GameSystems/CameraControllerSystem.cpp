#include "pch.h"
#include "CameraControllerSystem.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"


void CameraControllerSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

	if (cameraEntity.GetIdentifier() != NULL_ENTITY)
	{
		
	}
}