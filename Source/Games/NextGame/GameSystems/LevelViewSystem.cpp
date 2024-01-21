#include "pch.h"
#include "LevelViewSystem.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "Misc/CameraHelper.h"

void LevelViewSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [entity, levelViewComponent, viewController] : registry.Get<LevelViewComponent, ViewControllerComponent>())
	{
		// Do not want to allow input when not in level view
		if (!levelViewComponent->isActive)
		{
			continue;
		}

		Look(deltaTime, registry, entity, levelViewComponent, viewController);
		Zoom(deltaTime, registry, entity, levelViewComponent, viewController);
	}
}

void LevelViewSystem::Look(const float deltaTime, ECS::Registry& registry, ECS::EntityHandle handle, LevelViewComponent* levelView, ViewControllerComponent* viewController)
{
	AnalogInput input = Input::GetLeftControllerAnaloge(0);

	float keyInputX = Input::IsKeyHeld(viewController->rotateLeftKC) - Input::IsKeyHeld(viewController->rotateRightKC);
	float keyInputY = Input::IsKeyHeld(viewController->moveUpKC) - Input::IsKeyHeld(viewController->moveDownKC);

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(handle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		CameraComponent* cameraComponent = CameraHelper::GetMainCamera(registry);
		if (!cameraComponent)
		{
			LOGMESSAGE(Error, "Error finding main camera component");
			return;
		}

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), BMath::Vector3(cameraComponent->upVector));
		}
		else if (keyInputX)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(keyInputX), BMath::Vector3(cameraComponent->upVector));
		}
		

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(cameraComponent->rightVector));
		}
		else if (keyInputY)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), BMath::Vector3(cameraComponent->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}
}

void LevelViewSystem::Zoom(const float deltaTime, ECS::Registry& registry, ECS::EntityHandle, LevelViewComponent* levelView, ViewControllerComponent* viewController)
{
	AnalogInput input = Input::GetRightControllerAnaloge(0);
	float keyInput = Input::IsKeyHeld(viewController->zoomOutKC) - Input::IsKeyHeld(viewController->zoomInKC);
	// 	// No look input

	float delta = 0.0f;

	// Prioritize controller input
	if (input.y != 0.0f)
	{
		delta = -input.y * levelView->cameraMoveSpeed * deltaTime;
	}
	else if (keyInput != 0.0f)
	{
		delta = keyInput * levelView->cameraMoveSpeed * deltaTime;
	}
	else
	{
		return;
	}

	if (levelView->currentZoomCounter + delta < levelView->minZoomDistance || levelView->currentZoomCounter + delta > levelView->maxZoomDistance)
	{
		return;
	}

	levelView->currentZoomCounter += delta;

	CameraComponent* cameraComponent = CameraHelper::GetMainCamera(registry);
	if (!cameraComponent)
	{
		LOGMESSAGE(Error, "Error finding main camera component");
		return;
	}

	
	BMath::Vector3 deltaPosition = cameraComponent->forwardVector * delta;
	registry.AddComponent<MoveComponent>(levelView->attachedCamera, deltaPosition);
}
