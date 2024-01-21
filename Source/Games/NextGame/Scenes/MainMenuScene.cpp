#include "pch.h"
#include "MainMenuScene.h"
#include "HoleOneScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

#include "Systems/SystemManager.h"

#include "Factories/CameraFactory.h"
#include "Factories/SkySphereFactory.h"
#include "Factories/GameObjectFactory.h"

#include "Scripts/LevelHelper.h"
#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/GoalObject.h"
#include "Scripts/PlatformObject.h"
#include "Scripts/BarrierObject.h"
#include "Scripts/GolfUIHelper.h"
#include "Scripts/PlayerScore.h"


#include "GameSystems/PlayerFellSystem.h"
#include "GameSystems/PlayerRespawnSystem.h"
#include "GameSystems/LevelViewSystem.h"
#include "GameSystems/PlayerHUDSystem.h"
#include "GameSystems/MovingObsSystem.h"
#include "GameSystems/PCSystem.h"


MainMenuScene::MainMenuScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
	}

	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	if (ambientLight)
	{
		ambientLight->intensity = 2;
	}
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 5, 0));


	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 1.0f, 4));

	bottomOOBTrigger = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });
	registry.AddComponent<Behemoth::OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<Behemoth::StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<Behemoth::ScalingComponent>(bottomOOBTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<Behemoth::MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -20, 10.0f));

	if (Behemoth::MeshComponent* mesh = registry.GetComponent<Behemoth::MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	playEntity = GameObjectFactory::CreateGameObject(registry, "plane.obj", "play.png", "Play Game Entity", { 1,-1 });
	registry.AddComponent<MoveComponent>(playEntity, BMath::Vector3(3, 8, -15));
	registry.AddComponent<OBBColliderComponent>(playEntity, BMath::Vector3(3.0f));
	registry.AddComponent<Behemoth::StaticComponent>(playEntity);
	registry.AddComponent<Behemoth::ScalingComponent>(playEntity, BMath::Vector3(4.0f));
	registry.AddComponent<Behemoth::RotationComponent>(playEntity, BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(1, 0, 0)), true);


	titleTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(titleTextEntity, "Great Golf!", BMath::Vector2(-0.05f, 0.6f));
}

void MainMenuScene::Initalize()
{
	InitalizeSystems();
}

void MainMenuScene::Update(const float deltaTime)
{
	CheckOutOfBound(registry, playerCharacter, bottomOOBTrigger);

	if (CollisionDataComponent* collisionData = registry.GetComponent<CollisionDataComponent>(playerCharacter))
	{
		for (const auto& d : collisionData->data)
		{
			if (d.otherHandle.ID == playEntity.ID)
			{
				World::GetInstance().ChangeScene(new HoleOneScene());
			}
		}
	}
}

void MainMenuScene::OnEvent(Behemoth::Event& e)
{

}

void MainMenuScene::InitalizeSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<PCSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerFellSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<LevelViewSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerHUDSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<MovingObsSystem>();
}

void MainMenuScene::Shutdown()
{

}