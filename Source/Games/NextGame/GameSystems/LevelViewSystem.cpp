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

	// 	// No look input
	if (input == AnalogInput())
	{
		return;
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(handle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* springArmTransform = registry.GetComponent<TransformComponent>(handle);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), BMath::Vector3(springArmTransform->upVector));
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(springArmTransform->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}
}

void LevelViewSystem::Zoom(const float deltaTime, ECS::Registry& registry, ECS::EntityHandle, LevelViewComponent* levelView, ViewControllerComponent* viewController)
{
	AnalogInput input = Input::GetRightControllerAnaloge(0);

	// 	// No look input
	if (input.y == 0.0f)
	{
		return;
	}
	const float delta = -input.y * levelView->cameraMoveSpeed * deltaTime;

	if (levelView->currentZoomCounter + delta < levelView->minZoomDistance || levelView->currentZoomCounter + delta > levelView->maxZoomDistance)
	{
		return;
	}

	levelView->currentZoomCounter += delta;

	std::cout << levelView->currentZoomCounter << std::endl;
	BMath::Vector3 deltaPosition = CameraHelper::GetForwardVector(registry) * delta;
	registry.AddComponent<MoveComponent>(levelView->attachedCamera, deltaPosition);
}
