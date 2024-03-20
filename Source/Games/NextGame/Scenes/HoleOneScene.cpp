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
#include "Scripts/GoalViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/PlatformFactory.h"
#include "Scripts/BarrierFactory.h"
#include "Scripts/PlayerScore.h"

#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/goalViewComponent.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"

HoleOneScene::HoleOneScene() : GameScene(registry, 2)
{
	ConstructEnvironment(registry);

	goalViewHandle = GoalViewFactory::CreateGoalViewEntity(registry, BMath::Vector3(0, 2, -20),  20, 0);

	parTextHandle = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextHandle, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));
	playerHandle = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));
	PlayerScore::ResetScore();
}

void HoleOneScene::Initialize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required after constructor call
}

void HoleOneScene::Update(const float deltaTime)
{
	Super::Update(deltaTime);

	if (Behemoth::Input::IsControllerKeyDown(Behemoth::CC_Y) || Behemoth::Input::IsKeyDown(Behemoth::KC_C))
	{
		ViewMode::ToggleViewMode(registry, playerHandle, goalViewHandle);
	}

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new HoleTwoScene());
	}
}

void HoleOneScene::OnEvent(Behemoth::Event& e)
{

}

void HoleOneScene::Shutdown()
{

}

void HoleOneScene::ConstructEnvironment(ECS::Registry& registry)
{
	environmentLightingHandle = registry.CreateEntity("Environment Lighting");

	Behemoth::DirectionalLightComponent* directionalLightComp = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLightingHandle);
	if (directionalLightComp)
	{
		directionalLightComp->direction = BMath::Vector3(0.0f, 0.707, 0.707);
	}

	Behemoth::AmbientLightComponent* ambientLightComp = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLightingHandle);
	if (ambientLightComp)
	{
		ambientLightComp->intensity = 2;
	}

	skySphereHandle = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", 999.0f, BMath::Vector2(1.0, 1.0));

	goalHandle = CreateGoalObject(registry, BMath::Vector3(0, 2, -20), BMath::Vector3(3.0f), 0.0f);
	goalComp = registry.GetComponent<GoalComponent>(goalHandle);

	teeOffPlatformHandle = PlatformFactory::CreateGrassPlatform(
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

	grassPlatformHandle = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 0, 0),
		BMath::Vector3(10, 0.1f, 6));

	sandTrap1Handle = PlatformFactory::CreateSandPlatform(
		registry,
		BMath::Vector3(-20, -2, -20),
		BMath::Vector3(7, 0.1f, 10));

	sandTrap2Handle = PlatformFactory::CreateSandPlatform(
		registry,
		BMath::Vector3(20, -2, -20),
		BMath::Vector3(7, 0.1f, 10));
}
