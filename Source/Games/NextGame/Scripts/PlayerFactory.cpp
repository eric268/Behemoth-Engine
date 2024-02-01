#include "pch.h"
#include "PlayerFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/UIComponents.h"
#include "Misc/GameObjectHelper.h"

ECS::EntityHandle PlayerFactory::CreatePlayer(ECS::Registry& registry, const BMath::Vector3& spawnLocation)
{
	// Create the main player / root component of the player
	const ECS::EntityHandle playerHandle = registry.CreateEntity("Player");
	registry.AddComponent<Behemoth::RigidBodyComponent>(playerHandle, false, true);
	registry.AddComponent<Behemoth::MoveComponent>(playerHandle, spawnLocation);
	registry.AddComponent<Behemoth::SphereColliderComponent>(playerHandle);

	// Create a child component that will handle projectile aiming 
	const ECS::EntityHandle projectileHandle = registry.CreateEntity("Projectile");
	registry.AddComponent<Behemoth::TransformComponent>(projectileHandle);
	registry.AddComponent<Behemoth::RotationComponent>(projectileHandle);

	// Create child component that will be the projectile mesh
	const ECS::EntityHandle playerMeshHandle = Behemoth::GameObjectFactory::CreateGameObject(
		registry, 
		"sphere10.obj", 
		"golfball.png", 
		"Projectile Mesh");

	// Create an arrow child component to indicate aim direction
	const ECS::EntityHandle arrowMeshHandle = Behemoth::GameObjectFactory::CreateGameObject(
		registry, 
		"arrow.obj", 
		"arrow.jpg", 
		"Arrow icon");

	registry.AddComponent<Behemoth::ScalingComponent>(arrowMeshHandle, BMath::Vector3(0.25f));
	registry.AddComponent<Behemoth::MoveComponent>(arrowMeshHandle, BMath::Vector3(0, 0, -3));
	BMath::Quaternion q1 = BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(0, 0, 1));
	BMath::Quaternion q2 = BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(1, 0, 0));
	registry.AddComponent<Behemoth::RotationComponent>(arrowMeshHandle, q1 * q2);

	// Create player camera, used as main camera
	const ECS::EntityHandle mainCameraHandle = Behemoth::CameraFactory::CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0, 0, 10));
	Behemoth::CameraComponent* mainCameraComp = registry.GetComponent<Behemoth::CameraComponent>(mainCameraHandle);
	mainCameraComp->focusedEntity = playerHandle;

	// Create camera spring arm for look functionality independent of aiming
	const ECS::EntityHandle cameraSpringArmHandle = registry.CreateEntity("Spring Arm");
	registry.AddComponent<Behemoth::TransformComponent>(cameraSpringArmHandle);
	registry.AddComponent<Behemoth::RotationComponent>(cameraSpringArmHandle);

	// Add necessary child component to respective parents in hierarchy
	Behemoth::GameObjectHelper::AddChildObject(registry, playerHandle, projectileHandle);
	Behemoth::GameObjectHelper::AddChildObject(registry, projectileHandle, arrowMeshHandle);
	Behemoth::GameObjectHelper::AddChildObject(registry, projectileHandle, playerMeshHandle);
	Behemoth::GameObjectHelper::AddChildObject(registry, playerHandle, cameraSpringArmHandle);
	Behemoth::GameObjectHelper::AddChildObject(registry, cameraSpringArmHandle, mainCameraHandle);

	// Create and add a player component to easily reference child handles
	registry.AddComponent<PlayerComponent>(
		playerHandle
		,true,
		mainCameraHandle,
		cameraSpringArmHandle,
		projectileHandle,
		playerMeshHandle,
		arrowMeshHandle,
		20.0f,
		spawnLocation);

	// Bind input handling for player controller 
	AddInputBindings(registry, playerHandle);
	InitializeHUD(registry, playerHandle);

	return playerHandle;
}

void PlayerFactory::AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	registry.AddComponent<PCComponent>(
		entityHandle,
		Behemoth::KeyCode::KC_E,
		Behemoth::KeyCode::KC_Q,
		Behemoth::KeyCode::KC_Space,
		Behemoth::KeyCode::KC_Left,
		Behemoth::KeyCode::KC_Right,
		Behemoth::KeyCode::KC_Up,
		Behemoth::KeyCode::KC_Down,
		Behemoth::KeyCode::KC_A,
		Behemoth::KeyCode::KC_D,
		Behemoth::KeyCode::KC_W,
		Behemoth::KeyCode::KC_S,
		Behemoth::ControllerCode::CC_R_SHOULDER);
}

void PlayerFactory::InitializeHUD(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	PlayerHUDComponent* playerHUDComp = registry.AddComponent<PlayerHUDComponent>(entityHandle);
	if (!playerHUDComp)
	{
		LOGMESSAGE(Error, "Unable to add player HUD");
		return;
	}

	playerHUDComp->owningPlayerHandle = entityHandle;

	playerHUDComp->powerUIHandle = registry.CreateEntity("Player Power UI");
	registry.AddComponent<Behemoth::TextComponent>(playerHUDComp->powerUIHandle, "Power: ", BMath::Vector2(0.85, 0.9));

	playerHUDComp->strokesUIHandle = registry.CreateEntity("Player strokes UI");
	registry.AddComponent<Behemoth::TextComponent>(playerHUDComp->strokesUIHandle, "Strokes: 0", BMath::Vector2(0.85, 0.8));
}