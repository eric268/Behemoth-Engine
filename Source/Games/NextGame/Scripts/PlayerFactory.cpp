#include "pch.h"
#include "PlayerFactory.h"
#include "Factories/GameObjectFactory.h"
#include "GameComponents/Player/PCComponent.h"
#include "GameComponents/Player/PlayerComponent.h"

using namespace Behemoth;

ECS::EntityHandle PlayerFactory::CreatePlayer(ECS::Registry& registry, const BMath::Vector3& spawnLocation)
{
	GameObjectFactory gameObjectFactory{};

	// Create the main player / root component of the player
	const ECS::EntityHandle playerHandle = gameObjectFactory.CreateGameObject(registry, "", "", "Player");
	registry.AddComponent<RigidBodyComponent>(playerHandle, false, true);
	registry.AddComponent<MoveComponent>(playerHandle, spawnLocation);
	registry.AddComponent<SphereColliderComponent>(playerHandle);

	// Create a child component that will handle projectile aiming 
	const ECS::EntityHandle projectileHandle = registry.CreateEntity("Projectile");
	registry.AddComponent<TransformComponent>(projectileHandle);
	registry.AddComponent<RotationComponent>(projectileHandle);

	// Create child component that will be the projectile mesh
	const ECS::EntityHandle playerMeshHandle = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "rock.png", "Projectile Mesh");

	// Create an arrow child component to indicate aim direction
	const ECS::EntityHandle arrowMeshHandle = gameObjectFactory.CreateGameObject(registry, "arrow.obj", "arrow.jpg", "Arrow icon");
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
	gameObjectFactory.AddChildObject(registry, playerHandle, projectileHandle);
	gameObjectFactory.AddChildObject(registry, projectileHandle, arrowMeshHandle);
	gameObjectFactory.AddChildObject(registry, projectileHandle, playerMeshHandle);
	gameObjectFactory.AddChildObject(registry, playerHandle, cameraSpringArm);
	gameObjectFactory.AddChildObject(registry, cameraSpringArm, mainCameraHandle);

	// Create and add a player component to easily reference child handles
	registry.AddComponent<PlayerComponent>(playerHandle,true, mainCameraHandle, cameraSpringArm, projectileHandle, playerMeshHandle, arrowMeshHandle, 5.0f, spawnLocation);

	// Bind input handling for player controller 
	AddInputBindings(registry, playerHandle);

	return playerHandle;
}

void PlayerFactory::AddInputBindings(ECS::Registry& registry, const ECS::EntityHandle& handle)
{
	registry.AddComponent<PCComponent>(
		handle,
		KeyCode::KC_E,
		KeyCode::KC_Q,
		KeyCode::KC_Space,
		KeyCode::KC_C,
		ControllerCode::CC_R_SHOULDER,
		ControllerCode::CC_Y);
}