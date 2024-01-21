#include "pch.h"
#include "LevelViewFactory.h"
#include "Components/Components.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "Factories/CameraFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"

using namespace Behemoth;

ECS::EntityHandle LevelViewFactory::CreateLevelViewEntity(
	ECS::Registry& registry,
	const BMath::Vector3& spawnLocation,
	float maxZoom,
	float currentZoom,
	float maxHeight,
	float currentHeight)
{
	GameObjectFactory gameObjectFactory{};
	ECS::EntityHandle levelViewHandle = gameObjectFactory.CreateGameObject(registry, "", "", "Level view handle");
	registry.AddComponent<TransformComponent>(levelViewHandle);
	registry.AddComponent<MoveComponent>(levelViewHandle, spawnLocation);
	registry.AddComponent<RotationComponent>(levelViewHandle, BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(0,1,0)), true);

	CameraFactory cameraFactory{};
	ECS::EntityHandle cameraEntitiy = cameraFactory.CreateCamera(registry, false, "Level view camera");
	registry.AddComponent<MoveComponent>(cameraEntitiy, BMath::Vector3(0, currentHeight, currentZoom));
	CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(cameraEntitiy);
	if (cameraComponent)
	{
		cameraComponent->focusedEntity = levelViewHandle;
	}

	gameObjectFactory.AddChildObject(registry, levelViewHandle, cameraEntitiy);

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
		false,
		KeyCode::KC_W,
		KeyCode::KC_S,
		KeyCode::KC_A,
		KeyCode::KC_D,
		KeyCode::KC_E,
		KeyCode::KC_Q,
		ControllerCode::CC_DPAD_DOWN,
		ControllerCode::CC_DPAD_UP);
}