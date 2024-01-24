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
#include "Scripts/PlatformFactory.h"
#include "Scripts/BarrierFactory.h"
#include "Scripts/PlayerScore.h"

HoleTwoScene::HoleTwoScene()
{
	ConstructEnvironment(registry);

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));

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

void HoleTwoScene::Initialize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void HoleTwoScene::OnEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void HoleTwoScene::Update(const float deltaTime)
{
	CheckOutOfBound(registry, playerCharacter, oobTrigger);

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
		ambientLight->intensity = 0.8;
	}

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky3.png", { 1.0, 1.0 });

	teeOffPlatform = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 0.1f, 4));

	goalObject = CreateGoalObject(registry, BMath::Vector3(20, 15, -60), BMath::Vector3(3.0f), 45.0f);

	for (int i = 0; i < 3; i++)
	{
		mainBarriers[i] = BarrierFactory::CreateObstacle(registry, BMath::Vector3(-12 + (i * 20), 20, -30), BMath::Vector3(9, 15, 2), BMath::Quaternion());
	}

	grassPatch1 = PlatformFactory::CreateGrassPlatform(registry, BMath::Vector3(-40, 8, -20), BMath::Vector3(15, 0.1f, 40), BMath::Quaternion());
	grassPatch2 = PlatformFactory::CreateGrassPlatform(registry, BMath::Vector3(0, 8, -100), BMath::Vector3(25, 0.1f, 30), BMath::Quaternion());
	sandPatch2 = PlatformFactory::CreateSandPlatform(registry, BMath::Vector3(50, 8, -75), BMath::Vector3(20, 0.1f, 30), BMath::Quaternion());
	barrier1 = BarrierFactory::CreateObstacle(
		registry,
		BMath::Vector3(35, 20, -50),
		BMath::Vector3(5, 6, 10),
		BMath::Quaternion(DEGREE_TO_RAD(45.0f),
			BMath::Vector3(0, 1, 0)));
}

void HoleTwoScene::Shutdown()
{

}