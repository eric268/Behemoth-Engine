#include "pch.h"
#include "HoleThreeScene.h"
#include "GameOverScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

#include "Systems/SystemManager.h"

#include "Factories/CameraFactory.h"
#include "Factories/SkySphereFactory.h"
#include "Factories/GameObjectFactory.h"

#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/PlatformObject.h"
#include "Scripts/BarrierObject.h"
#include "Scripts/PlayerScore.h"


#include "GameSystems/PlayerFellSystem.h"
#include "GameSystems/PlayerRespawnSystem.h"
#include "GameSystems/LevelViewSystem.h"
#include "GameSystems/PlayerHUDSystem.h"
#include "GameSystems/MovingObsSystem.h"
#include "GameSystems/PCSystem.h"


HoleThreeScene::HoleThreeScene()
{
		environmentLighting = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.707, 0.707, 0.707).Normalize();
	}
	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	if (ambientLight)
	{
		ambientLight->intensity = 2;
	}
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky2.png", { 1.0, 1.0 });


	goalObject = CreateGoalObject(registry, BMath::Vector3(0, 10, -30), BMath::Vector3(3.0f), 45.0f);

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 15, 18));

	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 29, 18),
		BMath::Vector3(4, 1.0f, 4));

	CreateOOBEntity(registry);
	ConstructEnvironment(registry);

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(0, 2, -20), 5, 20, 10, 0);
	registry.AddComponent<Behemoth::TransformComponent>(levelViewEntity);
	registry.AddComponent<Behemoth::RotationComponent>(levelViewEntity);

	par = 3;
	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));

	delayUntilSceneChange = 3.0f;
	changeScene = false;
}
void HoleThreeScene::Initalize()
{
	InitalizeSystems();
}
void HoleThreeScene::Update(const float deltaTime)
{
	CheckOutOfBound(registry, playerCharacter, bottomOOBTrigger);

	if (CheckLevelComplete(registry, playerCharacter))
	{
		OnHoleComplete(registry, playerCharacter, par);

		ECS::EntityHandle changeSceneEntity = registry.CreateEntity("Change scene entity");
		registry.AddComponent<Behemoth::TimerComponent>(changeSceneEntity, delayUntilSceneChange, [this]()
			{
				changeScene = true;
			});
	}


	if (Behemoth::Input::IsControllerKeyDown(Behemoth::CC_Y) || Behemoth::Input::IsKeyDown(Behemoth::KC_C))
	{
		ViewModeChange::ChangeViewMode(registry, playerCharacter, levelViewEntity);
	}

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new GameOverScene());
	}
}
void HoleThreeScene::OnEvent(Behemoth::Event& e)
{

}
void HoleThreeScene::InitalizeSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<PCSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerFellSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<LevelViewSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerHUDSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<MovingObsSystem>();
}
void HoleThreeScene::Shutdown()
{

}

void HoleThreeScene::ConstructEnvironment(ECS::Registry& registry)
{
	auto rock1 = PlatformObject::CreateRockPlatform(registry, BMath::Vector3(0, 0, -10), BMath::Vector3(5, 1, 15));


	auto barrier = BarrierObject::CreateObstacle(registry, BMath::Vector3(0, 15, -10), BMath::Vector3(5, 6, 10), BMath::Quaternion());
}