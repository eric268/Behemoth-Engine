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
#include "GameSystems/GoalViewSystem.h"
#include "GameSystems/PlayerHUDSystem.h"
#include "GameSystems/MovingObsSystem.h"
#include "GameSystems/PCSystem.h"

MainMenuScene::MainMenuScene() : GameScene(registry, 0, 0.0f)
{
	ConstructEnvironment(registry);
	CreateInstructionsText(registry);
	playerHandle = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 5, 0));
	titleTextHandle = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(titleTextHandle, "Next Golf!", BMath::Vector2(-0.05f, 0.6f));
}

void MainMenuScene::Initialize()
{
	InitializeSystems();
}

void MainMenuScene::InitializeSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<PCSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerFallSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerRespawnSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<goalViewSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<PlayerHUDSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<MovingObsSystem>();
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

void MainMenuScene::Shutdown()
{

}

void MainMenuScene::ConstructEnvironment(ECS::Registry& registry)
{
	environmentLightingHandle = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLightingHandle);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
	}

	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLightingHandle);
	if (ambientLight)
	{
		ambientLight->intensity = 2.0f;
	}

	skySphereHandle = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", 999.0f, BMath::Vector2(1.0, 1.0));

	teeOffPlatformHandle = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 3.75f, 0),
		BMath::Vector3(4, 0.1f, 4));

	goalHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "plane5.obj", "play.png", "Play Game Entity", { 1,-1 });
	registry.AddComponent<Behemoth::MoveComponent>(goalHandle, BMath::Vector3(0, 10, -20));
	registry.AddComponent<Behemoth::AABBColliderComponent>(goalHandle, BMath::Vector3(1.0f, 1.0f, 0.1f));
	registry.AddComponent<Behemoth::StaticComponent>(goalHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(goalHandle, BMath::Vector3(4.0f));
 	registry.AddComponent<Behemoth::RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(1, 0, 0)));

	if (goalComp = registry.AddComponent<GoalComponent>(goalHandle))
	{
		goalComp->goalColliderHandles.push_back(goalHandle);
	}
}

void MainMenuScene::CreateInstructionsText(ECS::Registry& registry)
{
	// Controller instructions
	controllerInstructionsHandles[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[0], "Controller Controls", BMath::Vector2(-0.9, 0.8));

	controllerInstructionsHandles[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[1], "Look - Left Analog", BMath::Vector2(-0.9, 0.75));

	controllerInstructionsHandles[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[2], "Aim - Right Analog", BMath::Vector2(-0.9, 0.7));

	controllerInstructionsHandles[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[3], "Power Up/Down - Left & Right Triggers", BMath::Vector2(-0.9, 0.65));

	controllerInstructionsHandles[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[4], "Fire - Right Shoulder", BMath::Vector2(-0.9, 0.6));

	controllerInstructionsHandles[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(controllerInstructionsHandles[5], "Swap Camera View - Y Button (In-Game Only)", BMath::Vector2(-0.9, 0.55));


	// Controller instructions
	keyboardInstructionsHandles[0] = registry.CreateEntity("Instructions text 1");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[0], "Keyboard Controls", BMath::Vector2(-0.9, 0.4));

	keyboardInstructionsHandles[1] = registry.CreateEntity("Instructions text 2");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[1], "Look - WASD", BMath::Vector2(-0.9, 0.35));

	keyboardInstructionsHandles[2] = registry.CreateEntity("Instructions text 3");
	keyboardInstructionsHandles[2] = registry.CreateEntity("Instructions text 3");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[2], "Aim - Arrow Keys", BMath::Vector2(-0.9, 0.3));

	keyboardInstructionsHandles[3] = registry.CreateEntity("Instructions text 4");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[3], "Power  Up/Down - Q & E", BMath::Vector2(-0.9, 0.25));

	keyboardInstructionsHandles[4] = registry.CreateEntity("Instructions text 5");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[4], "Fire - Space", BMath::Vector2(-0.9, 0.2));

	keyboardInstructionsHandles[5] = registry.CreateEntity("Instructions text 6");
	registry.AddComponent<Behemoth::TextComponent>(keyboardInstructionsHandles[5], "Swap Camera View - C Key (In-Game Only)", BMath::Vector2(-0.9, 0.15));

	ECS::EntityHandle goalText = registry.CreateEntity("Goal Text");
	registry.AddComponent<Behemoth::TextComponent>(goalText, "Goal - Monkey Head", BMath::Vector2(-0.9, 0.05));
}
