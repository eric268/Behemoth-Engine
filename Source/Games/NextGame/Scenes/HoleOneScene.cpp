#include "pch.h"
#include "HoleOneScene.h"
#include "HoleTwoScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/AudioComponents.h"
#include "Components/UIComponents.h"

#include "GameSystems/PCSystem.h"

#include "Factories/SkySphereFactory.h"
#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/PlatformFactory.h"
#include "Scripts/BarrierFactory.h"
#include "Scripts/PlayerScore.h"

#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"

HoleOneScene::HoleOneScene() : GameScene(registry, 2)
{
	ConstructEnvironment(registry);

	levelViewEntity = LevelViewFactory::CreateLevelViewEntity(
		registry,
		BMath::Vector3(0, 2, -20),
		5, 
		20,
		10,
		0);

	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));
	PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerCharacter);
	PlayerScore::ResetScore();
}

void HoleOneScene::Initialize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void HoleOneScene::OnEvent(Behemoth::Event& e)
{
}

void HoleOneScene::Update(const float deltaTime)
{
	Super::Update(deltaTime);

	if (Behemoth::Input::IsControllerKeyDown(Behemoth::CC_Y) || Behemoth::Input::IsKeyDown(Behemoth::KC_C))
	{
		ViewMode::ToggleViewMode(registry, playerCharacter, levelViewEntity);
	}

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new HoleTwoScene());
	}
}

void HoleOneScene::Shutdown()
{

}

void HoleOneScene::ConstructEnvironment(ECS::Registry& registry)
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

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", BMath::Vector2(1.0, 1.0));

	goalHandle = CreateGoalObject(registry, BMath::Vector3(0, 2, -20), BMath::Vector3(3.0f), 0.0f);
	goalComponent = registry.GetComponent<GoalComponent>(goalHandle);

	teeOffPlatform = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 8.8f, 18),
		BMath::Vector3(4, 0.1f, 4));

	obstacleHandle = BarrierFactory::CreateObstacle(
		registry,
		BMath::Vector3(0, 5, -12),
		BMath::Vector3(3.0f, 3.0f, 1.0f), 
		BMath::Quaternion(),
		false);

	registry.AddComponent<MovingObsComponent>(obstacleHandle, BMath::Vector3::Up(), 30.0f, 250.0f, 0.0f);
	registry.AddComponent<Behemoth::VelocityComponent>(obstacleHandle);

	grassEntity = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 0, 0),
		BMath::Vector3(10, 0.1f, 6));

	sandTrap1 = PlatformFactory::CreateSandPlatform(
		registry,
		BMath::Vector3(-20, -2, -20),
		BMath::Vector3(7, 0.1f, 10));

	sandTrap2 = PlatformFactory::CreateSandPlatform(
		registry,
		BMath::Vector3(20, -2, -20),
		BMath::Vector3(7, 0.1f, 10));
}
