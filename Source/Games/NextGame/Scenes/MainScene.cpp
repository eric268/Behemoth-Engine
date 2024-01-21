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


#include "GameSystems/PlayerFellSystem.h"
#include "GameSystems/PlayerRespawnSystem.h"
#include "GameSystems/LevelViewSystem.h"
#include "GameSystems/PlayerHUDSystem.h"
#include "GameSystems/MovingObsSystem.h"

#include "Factories/SkySphereFactory.h"

#include "Physics/ProjectileMotion.h"

#include "GameSystems/PCSystem.h"
#include "Physics/Collision/PhysicsMaterial.h"

#include "TestScene.h"

#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/GoalObject.h"
#include "Scripts/PlatformObject.h"
#include "Scripts/BarrierObject.h"

#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"

using namespace Behemoth;

MainScene::MainScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 12));
	goalObject = GoalObject::CreateGoal(registry, BMath::Vector3(0, 2, -20), BMath::Vector3(3.0f), 45.0f);
	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 12),
		BMath::Vector3(4, 1.0f, 4));

	barrierHandle = BarrierObject::CreateBarrier(registry, BMath::Vector3(0, 5, -12), BMath::Vector3(3.0f, 3.0f, 1.0f), BMath::Quaternion(), false);
	registry.AddComponent<MovingObsComponent>(barrierHandle, BMath::Vector3::Up(), 20.0f, 100.0f, 50.0f);
	registry.AddComponent<Behemoth::VelocityComponent>(barrierHandle);
	registry.AddComponent<Behemoth::RigidBodyComponent>(barrierHandle);

	groundEntity = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0,0,0),
		BMath::Vector3(6, 1.0f, 8));



	sandTrap1 = PlatformObject::CreateSandPlatform(
		registry,
		BMath::Vector3(-20, -2, -20),
		BMath::Vector3(7, 1.0f, 10));

	sandTrap2 = PlatformObject::CreateSandPlatform(
		registry,
		BMath::Vector3(20, -2, -20),
		BMath::Vector3(7, 1.0f, 10));


	bottomOOBTrigger = GameObjectFactory::CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });
	registry.AddComponent<OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<ScalingComponent>(bottomOOBTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -20, 10.0f));
	// registry.AddComponent<WireframeComponent>(bottomOOBTrigger, "cube.obj");

	if (MeshComponent* mesh = registry.GetComponent<MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(0, 2, -20), 5, 20, 10, 0);
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

	if (CollisionDataComponent* collisionData = registry.GetComponent<CollisionDataComponent>(playerCharacter))
	{
		for (const auto& d : collisionData->data)
		{
			if (d.otherHandle.ID == goalObject.ID)
			{
				LOGMESSAGE(General, "Goal Reached!");
				// Open level complete scene

				if (VelocityComponent* velocity = registry.GetComponent<VelocityComponent>(playerCharacter))
				{
					velocity->velocity = BMath::Vector3::Zero();
				}

				if (RigidBodyComponent* rigidBodyComponent = registry.GetComponent<RigidBodyComponent>(playerCharacter))
				{
					rigidBodyComponent->affectedByGravity = false;
				}
				if (PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerCharacter))
				{
					playerComponent->levelComplete = true;
				}
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
	SystemManager::GetInstance().AddSystem<PlayerFellSystem>();
	SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
	SystemManager::GetInstance().AddSystem<LevelViewSystem>();
	SystemManager::GetInstance().AddSystem<PlayerHUDSystem>();
	SystemManager::GetInstance().AddSystem<MovingObsSystem>();
}

void MainScene::Shutdown()
{

}