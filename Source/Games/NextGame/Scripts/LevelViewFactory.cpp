#include "pch.h"
#include "LevelViewFactory.h"
#include "Components/Components.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "Factories/CameraFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "Misc/GameObjectHelper.h"

ECS::EntityHandle LevelViewFactory::CreateLevelViewEntity(
	ECS::Registry& registry,
	const BMath::Vector3& spawnLocation,
	float maxZoom,
	float currentZoom,
	float maxHeight,
	float currentHeight)
{
	ECS::EntityHandle levelViewHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "", "", "Level view handle");
	registry.AddComponent<Behemoth::TransformComponent>(levelViewHandle);
	registry.AddComponent<Behemoth::MoveComponent>(levelViewHandle, spawnLocation);
	registry.AddComponent<Behemoth::RotationComponent>(levelViewHandle, BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(0,1,0)), true);

	ECS::EntityHandle cameraEntitiy = Behemoth::CameraFactory::CreateCamera(registry, false, "Level view camera");
	registry.AddComponent<Behemoth::MoveComponent>(cameraEntitiy, BMath::Vector3(0, currentHeight, currentZoom));
	Behemoth::CameraComponent* cameraComponent = registry.GetComponent<Behemoth::CameraComponent>(cameraEntitiy);
	if (cameraComponent)
	{
		cameraComponent->focusedEntity = levelViewHandle;
	}

	Behemoth::GameObjectHelper::AddChildObject(registry, levelViewHandle, cameraEntitiy);

	LevelViewComponent* levelViewComponent = registry.AddComponent<LevelViewComponent>(
		levelViewHandle,
		false,
		5.0f,
		-currentZoom + 5.0f,
		currentZoom * 2.0f,
		0.0f,
		0.0f,
		0.0f);

	if (levelViewComponent)
	{
		levelViewComponent->attachedCamera = cameraEntitiy;
		levelViewComponent->cameraMoveSpeed = 5.0f;
	}

	AddInputBindings(registry, levelViewHandle);

	return levelViewHandle;
}


void LevelViewFactory::AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle)
{
	registry.AddComponent<ViewControllerComponent>(handle,
		Behemoth::KeyCode::KC_A,
		Behemoth::KeyCode::KC_D,
		Behemoth::KeyCode::KC_W,
		Behemoth::KeyCode::KC_S);
}