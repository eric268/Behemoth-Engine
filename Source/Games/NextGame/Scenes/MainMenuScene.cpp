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


MainMenuScene::MainMenuScene()
{
	ConstructEnvironment(registry);
	

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 5, 0));

	CreateOOBEntity(registry);

	CreateInstructionsText(registry);

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

	if (CheckLevelComplete(registry, playerCharacter))
	{
		World::GetInstance().ChangeScene(new HoleOneScene());
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

void MainMenuScene::ConstructEnvironment(ECS::Registry& registry)
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

	goalObject = GameObjectFactory::CreateGameObject(registry, "p5.obj", "play.png", "Play Game Entity", { 1,-1 });
	registry.AddComponent<MoveComponent>(goalObject, BMath::Vector3(0, 10, -20));
	registry.AddComponent<OBBColliderComponent>(goalObject, BMath::Vector3(3.0f));
	registry.AddComponent<Behemoth::StaticComponent>(goalObject);
	registry.AddComponent<Behemoth::ScalingComponent>(goalObject, BMath::Vector3(4.0f));
	registry.AddComponent<Behemoth::RotationComponent>(goalObject, BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(1, 0, 0)), true);

	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 4, 0),
		BMath::Vector3(4, 1.0f, 4));
}

void MainMenuScene::CreateInstructionsText(ECS::Registry& registry)
{
	// Controller instructions
	controllerInstructions[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[0], "Controller Controls", BMath::Vector2(-0.9, 0.8));

	controllerInstructions[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[1], "Look - Left analog", BMath::Vector2(-0.9, 0.75));

	controllerInstructions[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[2], "Aim - Right analog", BMath::Vector2(-0.9, 0.7));

	controllerInstructions[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[3], "Power  up/down - left & right triggers", BMath::Vector2(-0.9, 0.65));

	controllerInstructions[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[4], "Fire - Right shoulder", BMath::Vector2(-0.9, 0.6));

	controllerInstructions[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[5], "Swap camera view - Y Button (In-game only)", BMath::Vector2(-0.9, 0.55));


	// Controller instructions
	keyboardInstructions[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[0], "Keyboard Controls", BMath::Vector2(-0.9, 0.4));

	keyboardInstructions[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[1], "Look - Arrow Keys", BMath::Vector2(-0.9, 0.35));

	keyboardInstructions[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[2], "Aim - WASD", BMath::Vector2(-0.9, 0.3));

	keyboardInstructions[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[3], "Power  up/down - Q & E", BMath::Vector2(-0.9, 0.25));

	keyboardInstructions[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[4], "Fire - Space", BMath::Vector2(-0.9, 0.2));

	keyboardInstructions[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[5], "Swap camera view - C Key (In-game only)", BMath::Vector2(-0.9, 0.15));

}
