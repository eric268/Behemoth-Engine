#include "pch.h"
#include "GoalViewSystem.h"
#include "GameComponents/Level/goalViewComponent.h"
#include "Misc/CameraHelper.h"
#include "GameComponents/Level/goalViewComponent.h"
#include "GameComponents/Player/PCComponent.h"

void goalViewSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entityHandle,
			goalViewComp,
			goalViewControllerComp] : registry.Get<GoalViewComponent, GoalViewControllerComponent>())
	{
		// Do not want to allow input when not in level view
		if (!goalViewComp->isActive)
		{
			continue;
		}
		Look(deltaTime, registry, entityHandle, goalViewComp, goalViewControllerComp);
	}
}

void goalViewSystem::Look(
	const float deltaTime, 
	ECS::Registry& registry, 
	const ECS::EntityHandle& entityHandle,
	GoalViewComponent* goalViewComp,
	GoalViewControllerComponent* goalViewControllerComp)
{
	Behemoth::AnalogInput input = Behemoth::Input::GetLeftControllerAnalog(0);

	float keyInputX = Behemoth::Input::IsKeyHeld(goalViewControllerComp->rotateLeftKC) - Behemoth::Input::IsKeyHeld(goalViewControllerComp->rotateRightKC);
	float keyInputY = Behemoth::Input::IsKeyHeld(goalViewControllerComp->moveUpKC)     - Behemoth::Input::IsKeyHeld(goalViewControllerComp->moveDownKC);

	if (Behemoth::RotationComponent* parentRotationComp = registry.AddComponent<Behemoth::RotationComponent>(entityHandle))
	{
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		Behemoth::CameraComponent* cameraComp = Behemoth::CameraHelper::GetMainCamera(registry);
		if (!cameraComp)
		{
			LOGMESSAGE(Error, "Error finding main camera component");
			return;
		}

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(-input.x), BMath::Vector3(cameraComp->upVector));
		}
		else if (keyInputX)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(keyInputX), BMath::Vector3(cameraComp->upVector));
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(input.y), BMath::Vector3(cameraComp->rightVector));
		}
		else if (keyInputY)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(keyInputY), BMath::Vector3(cameraComp->rightVector));
		}

		parentRotationComp->quat = quatY * quatX;
		parentRotationComp->isAdditive = true;
	}
}
