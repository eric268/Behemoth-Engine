#include "pch.h"
#include "PlayerFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/UIComponents.h"

using namespace Behemoth;

ECS::EntityHandle PlayerFactory::CreatePlayer(ECS::Registry& registry, const BMath::Vector3& spawnLocation)
{
	// Create the main player / root component of the player
	const ECS::EntityHandle playerHandle = GameObjectFactory::CreateGameObject(registry, "", "", "Player");
	registry.AddComponent<RigidBodyComponent>(playerHandle, false, true);
	registry.AddComponent<MoveComponent>(playerHandle, spawnLocation);
	registry.AddComponent<SphereColliderComponent>(playerHandle);

	// Create a child component that will handle projectile aiming 
	const ECS::EntityHandle projectileHandle = registry.CreateEntity("Projectile");
	registry.AddComponent<TransformComponent>(projectileHandle);
	registry.AddComponent<RotationComponent>(projectileHandle);
	registry.AddComponent<OBBColliderComponent>(playerHandle, BMath::Vector3(0.75f));

	// Create child component that will be the projectile mesh
	const ECS::EntityHandle playerMeshHandle = GameObjectFactory::CreateGameObject(registry, "sphere10.obj", "golfball.png", "Projectile Mesh");

	// Create an arrow child component to indicate aim direction
	const ECS::EntityHandle arrowMeshHandle = GameObjectFactory::CreateGameObject(registry, "arrow.obj", "arrow.jpg", "Arrow icon");
	registry.AddComponent<ScalingComponent>(arrowMeshHandle, BMath::Vector3(0.25));
	registry.AddComponent<MoveComponent>(arrowMeshHandle, BMath::Vector3(0, 0, -3));
	BMath::Quaternion q1 = BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(0, 0, 1));
	BMath::Quaternion q2 = BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(1, 0, 0));
	registry.AddComponent<RotationComponent>(arrowMeshHandle, q1 * q2, true);

	// Create player camera, used as main camera
	Behemoth::CameraFactory cameraFactory{};
	const ECS::EntityHandle mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<MoveComponent>(mainCameraHandle, BMath::Vector3(0, 0, 10));
	CameraComponent* mainCameraComp = registry.GetComponent<CameraComponent>(mainCameraHandle);
	mainCameraComp->focusedEntity = playerHandle;


	// Create camera spring arm for look functionality independent of aiming
	const ECS::EntityHandle cameraSpringArm = registry.CreateEntity("Spring arm");
	registry.AddComponent<TransformComponent>(cameraSpringArm);
	registry.AddComponent<RotationComponent>(cameraSpringArm);

	// Add necessary child component to respective parents in hierarchy
	GameObjectFactory::AddChildObject(registry, playerHandle, projectileHandle);
	GameObjectFactory::AddChildObject(registry, projectileHandle, arrowMeshHandle);
	GameObjectFactory::AddChildObject(registry, projectileHandle, playerMeshHandle);
	GameObjectFactory::AddChildObject(registry, playerHandle, cameraSpringArm);
	GameObjectFactory::AddChildObject(registry, cameraSpringArm, mainCameraHandle);

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
		KeyCode::KC_E,
		KeyCode::KC_Q,
		KeyCode::KC_Space,
		KeyCode::KC_Left,
		KeyCode::KC_Right,
		KeyCode::KC_Up,
		KeyCode::KC_Down,
		KeyCode::KC_A,
		KeyCode::KC_D,
		KeyCode::KC_W,
		KeyCode::KC_S,
		ControllerCode::CC_R_SHOULDER);
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
	registry.AddComponent<TextComponent>(playerHUD->powerUIHandle, "Power: ", BMath::Vector2(0.85, 0.9));

	playerHUD->strokesUIHandle = registry.CreateEntity("Player strokes UI");
	registry.AddComponent<TextComponent>(playerHUD->strokesUIHandle, "Strokes: 0", BMath::Vector2(0.85, 0.8));
}