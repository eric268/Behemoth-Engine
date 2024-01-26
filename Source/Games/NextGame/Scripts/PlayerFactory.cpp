#include "pch.h"
#include "PlayerFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/UIComponents.h"


ECS::EntityHandle PlayerFactory::CreatePlayer(ECS::Registry& registry, const BMath::Vector3& spawnLocation)
{
	// Create the main player / root component of the player
	const ECS::EntityHandle playerHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "", "", "Player");
	registry.AddComponent<Behemoth::RigidBodyComponent>(playerHandle, false, true);
	registry.AddComponent<Behemoth::MoveComponent>(playerHandle, spawnLocation);
	registry.AddComponent<Behemoth::SphereColliderComponent>(playerHandle);

	// Create a child component that will handle projectile aiming 
	const ECS::EntityHandle projectileHandle = registry.CreateEntity("Projectile");
	registry.AddComponent<Behemoth::TransformComponent>(projectileHandle);
	registry.AddComponent<Behemoth::RotationComponent>(projectileHandle);

	// Create child component that will be the projectile mesh
	const ECS::EntityHandle playerMeshHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "sphere10.obj", "golfball.png", "Projectile Mesh");

	// Create an arrow child component to indicate aim direction
	const ECS::EntityHandle arrowMeshHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "arrow.obj", "arrow.jpg", "Arrow icon");
	registry.AddComponent<Behemoth::ScalingComponent>(arrowMeshHandle, BMath::Vector3(0.25));
	registry.AddComponent<Behemoth::MoveComponent>(arrowMeshHandle, BMath::Vector3(0, 0, -3));
	BMath::Quaternion q1 = BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(0, 0, 1));
	BMath::Quaternion q2 = BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(1, 0, 0));
	registry.AddComponent<Behemoth::RotationComponent>(arrowMeshHandle, q1 * q2);

	// Create player camera, used as main camera
	Behemoth::CameraFactory cameraFactory{};
	const ECS::EntityHandle mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0, 0, 10));
	Behemoth::CameraComponent* mainCameraComp = registry.GetComponent<Behemoth::CameraComponent>(mainCameraHandle);
	mainCameraComp->focusedEntity = playerHandle;

	// Create camera spring arm for look functionality independent of aiming
	const ECS::EntityHandle cameraSpringArm = registry.CreateEntity("Spring arm");
	registry.AddComponent<Behemoth::TransformComponent>(cameraSpringArm);
	registry.AddComponent<Behemoth::RotationComponent>(cameraSpringArm);

	// Add necessary child component to respective parents in hierarchy
	Behemoth::GameObjectFactory::AddChildObject(registry, playerHandle, projectileHandle);
	Behemoth::GameObjectFactory::AddChildObject(registry, projectileHandle, arrowMeshHandle);
	Behemoth::GameObjectFactory::AddChildObject(registry, projectileHandle, playerMeshHandle);
	Behemoth::GameObjectFactory::AddChildObject(registry, playerHandle, cameraSpringArm);
	Behemoth::GameObjectFactory::AddChildObject(registry, cameraSpringArm, mainCameraHandle);

	// Create and add a player component to easily reference child handles
	registry.AddComponent<PlayerComponent>(
		playerHandle
		,true,
		mainCameraHandle,
		cameraSpringArm,
		projectileHandle,
		playerMeshHandle,
		arrowMeshHandle,
		20.0f,
		spawnLocation);

	// Bind input handling for player controller 
	AddInputBindings(registry, playerHandle);

	InitalizeHUD(registry, playerHandle);

	return playerHandle;
}

void PlayerFactory::AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle)
{
	registry.AddComponent<PCComponent>(
		handle,
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

void PlayerFactory::InitalizeHUD(ECS::Registry& registry, const ECS::EntityHandle& handle)
{
	PlayerHUDComponent* playerHUD = registry.AddComponent<PlayerHUDComponent>(handle);
	if (!playerHUD)
	{
		LOGMESSAGE(Error, "Unable to add player HUD");
		return;
	}

	playerHUD->owningPlayerHandle = handle;

	playerHUD->powerUIHandle = registry.CreateEntity("Player Power UI");
	registry.AddComponent<Behemoth::TextComponent>(playerHUD->powerUIHandle, "Power: ", BMath::Vector2(0.85, 0.9));

	playerHUD->strokesUIHandle = registry.CreateEntity("Player strokes UI");
	registry.AddComponent<Behemoth::TextComponent>(playerHUD->strokesUIHandle, "Strokes: 0", BMath::Vector2(0.85, 0.8));
}