#include "pch.h"
#include "GoalViewFactory.h"
#include "Components/Components.h"
#include "GameComponents/Level/GoalViewComponent.h"
#include "Factories/CameraFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "Misc/GameObjectHelper.h"

ECS::EntityHandle GoalViewFactory::CreateGoalViewEntity(
	ECS::Registry& registry,
	const BMath::Vector3& spawnLocation,
	float currentZoom,
	float currentHeight)
{
	ECS::EntityHandle goalViewHandle = registry.CreateEntity("Goal view");
	registry.AddComponent<Behemoth::TransformComponent>(goalViewHandle);
	registry.AddComponent<Behemoth::MoveComponent>(goalViewHandle, spawnLocation);
	registry.AddComponent<Behemoth::RotationComponent>(goalViewHandle, BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(0,1,0)), true);

	ECS::EntityHandle cameraEntityHandle = Behemoth::CameraFactory::CreateCamera(registry, false, "Goal view camera");
	registry.AddComponent<Behemoth::MoveComponent>(cameraEntityHandle, BMath::Vector3(0, currentHeight, currentZoom));

	if (Behemoth::CameraComponent* cameraComp = registry.GetComponent<Behemoth::CameraComponent>(cameraEntityHandle))
	{
		cameraComp->focusedEntity = goalViewHandle;
	}

	Behemoth::GameObjectHelper::AddChildObject(registry, goalViewHandle, cameraEntityHandle);

	GoalViewComponent* goalViewComp = registry.AddComponent<GoalViewComponent>(
		goalViewHandle,
		false,
		5.0f,
		-currentZoom + 5.0f,
		currentZoom * 2.0f,
		0.0f,
		0.0f,
		0.0f);

	if (goalViewComp)
	{
		goalViewComp->attachedCamera = cameraEntityHandle;
		goalViewComp->cameraMoveSpeed = 5.0f;
	}

	AddInputBindings(registry, goalViewHandle);

	return goalViewHandle;
}


void GoalViewFactory::AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle)
{
	registry.AddComponent<ViewControllerComponent>(handle,
		Behemoth::KeyCode::KC_A,
		Behemoth::KeyCode::KC_D,
		Behemoth::KeyCode::KC_W,
		Behemoth::KeyCode::KC_S);
}