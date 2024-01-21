#include "pch.h"
#include "HoleTwoScene.h"
#include "HoleThreeScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

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


HoleTwoScene::HoleTwoScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
		directionalLight->intensity = 1.8f;
	}
	
	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	if (ambientLight)
	{
		ambientLight->intensity = 0.6;
	}
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky3.png", { 1.0, 1.0 });

	ConstructEnvironment(registry);

	goalObject = CreateGoalObject(registry, BMath::Vector3(20, 15, -60), BMath::Vector3(3.0f), 45.0f);

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));

	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 1.0f, 4));

	CreateOOBEntity(registry);

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(20, 15, -60), 5, 20, 10, 0);
	registry.AddComponent<Behemoth::TransformComponent>(levelViewEntity);
	registry.AddComponent<Behemoth::RotationComponent>(levelViewEntity);

	par = 4;
	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));

	delayUntilSceneChange = 3.0f;
	changeScene = false;
}

void HoleTwoScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction

	Behemoth::SystemManager::GetInstance().AddSystem<PCSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerFellSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<LevelViewSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerHUDSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<MovingObsSystem>();
}

void HoleTwoScene::OnEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void HoleTwoScene::Update(const float deltaTime)
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
		Behemoth::World::GetInstance().ChangeScene(new HoleThreeScene());
	}
}

void HoleTwoScene::ConstructEnvironment(ECS::Registry& registry)
{
	for (int i = 0; i < 3; i++)
	{
		mainBarriers[i] = BarrierObject::CreateObstacle(registry, BMath::Vector3(-12 + (i * 20), 20, -30), BMath::Vector3(9, 15, 2), BMath::Quaternion());
	}



	grassPatch1 = PlatformObject::CreateGrassPlatform(registry, BMath::Vector3(-40, 8, -20), BMath::Vector3(15, 0.2, 40), BMath::Quaternion());

	auto grassPatch2 = PlatformObject::CreateGrassPlatform(registry, BMath::Vector3(0, 8, -100), BMath::Vector3(25, 0.2, 30), BMath::Quaternion());


	auto sandPatch2 = PlatformObject::CreateSandPlatform(registry, BMath::Vector3(50, 8, -95), BMath::Vector3(20, 0.2, 30), BMath::Quaternion());

	auto barrier2 = BarrierObject::CreateObstacle(
		registry,
		BMath::Vector3(35, 20, -50),
		BMath::Vector3(5, 6, 10),
		BMath::Quaternion(DEGREE_TO_RAD(45.0f),
			BMath::Vector3(0, 1, 0)));


}

void HoleTwoScene::Shutdown()
{

}