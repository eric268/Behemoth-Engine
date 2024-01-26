#include "pch.h"
#include "MainMenuScene.h"
#include "HoleOneScene.h"

#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

#include "Systems/SystemManager.h"

#include "Factories/CameraFactory.h"
#include "Factories/SkySphereFactory.h"
#include "Factories/GameObjectFactory.h"

#include "Scripts/PlayerFactory.h"
#include "Scripts/PlatformFactory.h"

#include "GameSystems/PlayerFallSystem.h"
#include "GameSystems/PlayerRespawnSystem.h"
#include "GameSystems/LevelViewSystem.h"
#include "GameSystems/PlayerHUDSystem.h"
#include "GameSystems/MovingObsSystem.h"
#include "GameSystems/PCSystem.h"

MainMenuScene::MainMenuScene() : GameScene(registry, 0, 0.0f)
{
	ConstructEnvironment(registry);
	CreateInstructionsText(registry);
	
	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 5, 0));

	titleTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(titleTextEntity, "Next Golf!", BMath::Vector2(-0.05f, 0.6f));
}

void MainMenuScene::Initialize()
{
	InitializeSystems();
}

void MainMenuScene::Update(const float deltaTime)
{
	Super::Update(deltaTime);

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new HoleOneScene());
	}
}

void MainMenuScene::OnEvent(Behemoth::Event& e)
{

}

void MainMenuScene::InitializeSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<PCSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerFallSystem>();
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
		ambientLight->intensity = 2.0f;
	}

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", BMath::Vector2(1.0, 1.0));

	teeOffPlatform = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 3.75f, 0),
		BMath::Vector3(4, 0.1f, 4));

	goalHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "plane5.obj", "play.png", "Play Game Entity", { 1,-1 });
	registry.AddComponent<Behemoth::MoveComponent>(goalHandle, BMath::Vector3(0, 10, -20));
	registry.AddComponent<Behemoth::AABBColliderComponent>(goalHandle, BMath::Vector3(1.0f, 1.0f, 0.1f));
	registry.AddComponent<Behemoth::StaticComponent>(goalHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(goalHandle, BMath::Vector3(4.0f));
 	registry.AddComponent<Behemoth::RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(1, 0, 0)));

	if (goalComponent = registry.AddComponent<GoalComponent>(goalHandle))
	{
		goalComponent->goalColliderHandles.push_back(goalHandle);
	}
}

void MainMenuScene::CreateInstructionsText(ECS::Registry& registry)
{
	// Controller instructions
	controllerInstructions[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[0], "Controller Controls", BMath::Vector2(-0.9, 0.8));

	controllerInstructions[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[1], "Look - Left Analog", BMath::Vector2(-0.9, 0.75));

	controllerInstructions[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[2], "Aim - Right Analog", BMath::Vector2(-0.9, 0.7));

	controllerInstructions[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[3], "Power Up/Down - Left & Right Triggers", BMath::Vector2(-0.9, 0.65));

	controllerInstructions[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[4], "Fire - Right Shoulder", BMath::Vector2(-0.9, 0.6));

	controllerInstructions[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructions[5], "Swap Camera View - Y Button (In-Game Only)", BMath::Vector2(-0.9, 0.55));


	// Controller instructions
	keyboardInstructions[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[0], "Keyboard Controls", BMath::Vector2(-0.9, 0.4));

	keyboardInstructions[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[1], "Look - WASD", BMath::Vector2(-0.9, 0.35));

	keyboardInstructions[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[2], "Aim - Arrow Keys", BMath::Vector2(-0.9, 0.3));

	keyboardInstructions[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[3], "Power  Up/Down - Q & E", BMath::Vector2(-0.9, 0.25));

	keyboardInstructions[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[4], "Fire - Space", BMath::Vector2(-0.9, 0.2));

	keyboardInstructions[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructions[5], "Swap Camera View - C Key (In-Game Only)", BMath::Vector2(-0.9, 0.15));

	ECS::EntityHandle goalText = registry.CreateEntity("Goal Text");
	registry.AddComponent<Behemoth::TextComponent>(goalText, "Goal - Monkey Head", BMath::Vector2(-0.9, 0.05));
}
