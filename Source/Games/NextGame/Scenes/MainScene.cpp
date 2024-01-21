#include "pch.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Components/AudioComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "Components/UIComponents.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"
#include "GameSystems/PlayerFellSystem.h"
#include "GameSystems/PlayerRespawnSystem.h"

#include "Factories/SkySphereFactory.h"

#include "Physics/ProjectileMotion.h"

#include "GameSystems/PCSystem.h"
#include "Physics/Collision/PhysicsMaterial.h"

#include "TestScene.h"

#include "Scripts/GameObjects/PlayerFactory.h"
#include "Scripts/GameObjects/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"

#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/LevelComponents.h"

using namespace Behemoth;

MainScene::MainScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 10));

	GameObjectFactory gameObjectFactory{};
	obstacleHandle = gameObjectFactory.CreateGameObject(registry, "cube.obj", "brick.png");
	registry.AddComponent<ScalingComponent>(obstacleHandle, BMath::Vector3(3, 3, 1.5));
	registry.AddComponent<MoveComponent>(obstacleHandle, BMath::Vector3(-5, 10, 0));
	registry.AddComponent<RotationComponent>(obstacleHandle, BMath::Quaternion(DEGREE_TO_RAD(25.0f), BMath::Vector3(0,1,0)), true);
	registry.AddComponent<StaticComponent>(obstacleHandle);
	auto obstacleCollider = registry.AddComponent<OBBColliderComponent>(obstacleHandle);
	obstacleCollider->physicsMaterial = PhysicsMaterial(0.75, 1.0f);

	ECS::EntityHandle obstacleHandle2 = gameObjectFactory.CreateGameObject(registry, "cube.obj", "brick.png");
	registry.AddComponent<ScalingComponent>(obstacleHandle2, BMath::Vector3(3, 3, 1.5));
	registry.AddComponent<MoveComponent>(obstacleHandle2, BMath::Vector3(7, 10, 0));
	registry.AddComponent<RotationComponent>(obstacleHandle2, BMath::Quaternion(DEGREE_TO_RAD(-25.0f), BMath::Vector3(0, 1, 0)), true);
	registry.AddComponent<StaticComponent>(obstacleHandle2);
	auto obstacleCollider2 = registry.AddComponent<OBBColliderComponent>(obstacleHandle2);
	obstacleCollider2->physicsMaterial = PhysicsMaterial(0.75, 1.0f);

	

	groundEntity = gameObjectFactory.CreateGameObject(registry, "plane.obj", "brick.png", "Ground entity", { 8,8 });
	registry.AddComponent<MoveComponent>(groundEntity, BMath::Vector3(0, 0, 0));
	registry.AddComponent<ScalingComponent>(groundEntity, BMath::Vector3(10, 1.1f, 10));
	auto groundCollider = registry.AddComponent<OBBColliderComponent>(groundEntity, BMath::Vector3(1.0f));
	groundCollider->physicsMaterial = PhysicsMaterial(0.85, 0.95);
	registry.AddComponent<StaticComponent>(groundEntity);
	registry.AddComponent<RotationComponent>(groundEntity, BMath::Quaternion(DEGREE_TO_RAD(1), BMath::Vector3(1,0,0)));
	// registry.AddComponent<WireframeComponent>(groundEntity, "plane.obj", BMath::Vector3(1, 0.1, 1));


	bottomOOBTrigger = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });
	registry.AddComponent<OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<ScalingComponent>(bottomOOBTrigger, BMath::Vector3(100, 1.0f, 100.0));
	registry.AddComponent<MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -10, 10.0f));
	registry.AddComponent<WireframeComponent>(bottomOOBTrigger, "cube.obj");
	if (MeshComponent* mesh = registry.GetComponent<MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(0.0f, 20.0f, -5.0f), 5, 20, 10, 0);

	registry.AddComponent<TransformComponent>(levelViewEntity);
	registry.AddComponent<RotationComponent>(levelViewEntity);
}

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
	InitalizeSystems();
}

void MainScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void MainScene::Update(const float deltaTime)
{
	if (TriggerDataComponent* triggerData = registry.GetComponent<TriggerDataComponent>(playerCharacter))
	{
		for (const auto& d : triggerData->data)
		{
			if (d.otherHandle.ID == bottomOOBTrigger.ID)
			{
				registry.AddComponent<PlayerFellComponent>(playerCharacter, 3.0f);
			}
		}
	}

	if (Input::IsControllerKeyDown(CC_Y) || Input::IsKeyDown(KC_C))
	{
		ViewModeChange::ChangeViewMode(registry, playerCharacter, levelViewEntity);
	}
}

void MainScene::InitalizeSystems()
{
	SystemManager::GetInstance().AddSystem<PCSystem>();
	SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
	SystemManager::GetInstance().AddSystem<PlayerFellSystem>();
	SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
}

void MainScene::Shutdown()
{

}