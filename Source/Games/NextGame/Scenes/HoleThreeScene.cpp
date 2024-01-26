#include "pch.h"
#include "HoleThreeScene.h"
#include "GameOverScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"

#include "Systems/SystemManager.h"

#include "Factories/CameraFactory.h"
#include "Factories/SkySphereFactory.h"
#include "Factories/GameObjectFactory.h"

#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/PlatformFactory.h"
#include "Scripts/BarrierFactory.h"
#include "Scripts/PlayerScore.h"

HoleThreeScene::HoleThreeScene() : GameScene(registry, 3)
{
	ConstructEnvironment(registry);
	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 15, 18));
	levelViewEntity = LevelViewFactory::CreateLevelViewEntity(
		registry, 
		BMath::Vector3(0, 10, -30),
		5,
		20,
		10,
		0);

	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));
}
void HoleThreeScene::Initialize()
{
	InitializeSystems();
}
void HoleThreeScene::Update(const float deltaTime)
{
	Super::Update(deltaTime);

	if (Behemoth::Input::IsControllerKeyDown(Behemoth::CC_Y) || Behemoth::Input::IsKeyDown(Behemoth::KC_C))
	{
		ViewMode::ToggleViewMode(registry, playerCharacter, levelViewEntity);
	}

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new GameOverScene());
	}
}
void HoleThreeScene::OnEvent(Behemoth::Event& e)
{

}
void HoleThreeScene::InitializeSystems()
{
}
void HoleThreeScene::Shutdown()
{

}

void HoleThreeScene::ConstructEnvironment(ECS::Registry& registry)
{
	environmentLighting = registry.CreateEntity("Environment Lighting");

	if (Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting))
	{
		directionalLight->direction = BMath::Vector3(0.707, 0.707, 0.707).Normalize();
	}

	if (Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting))
	{
		ambientLight->intensity = 2;
	}

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky2.png", BMath::Vector2(1.0, 1.0));

	rockPlatform1 = PlatformFactory::CreateRockPlatform(registry, BMath::Vector3(0, 0, -10), BMath::Vector3(5, 1, 15));

	barrier1 = BarrierFactory::CreateObstacle(
		registry, 
		BMath::Vector3(0, 15, -0),
		BMath::Vector3(5, 6, 1), 
		BMath::Quaternion(),
		false);

	registry.AddComponent<MovingObsComponent>(barrier1, BMath::Vector3(0, 1, 0), 40.0f, 250.0f, 0.0f);


	goalHandle = CreateGoalObject(registry, BMath::Vector3(0, 10, -30), BMath::Vector3(3.0f), 0.0f);
	goalComponent = registry.GetComponent<GoalComponent>(goalHandle);
// 
// 	teeOffPlatform = PlatformObject::CreateGrassPlatform(
// 		registry,
// 		BMath::Vector3(0, 14, 18),
// 		BMath::Vector3(4, 1.0f, 4));
}