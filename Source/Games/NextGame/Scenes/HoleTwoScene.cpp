#include "pch.h"
#include "HoleTwoScene.h"
#include "HoleThreeScene.h"

#include "Scripts/PlayerScore.h"
#include "Components/LightComponents.h"
#include "Components/RenderComponents.h"
#include "Components/UIComponents.h"

#include "Scripts/PlayerFactory.h"
#include "Scripts/GoalViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/PlatformFactory.h"
#include "Scripts/BarrierFactory.h"
#include "Scripts/PlayerScore.h"

HoleTwoScene::HoleTwoScene() : GameScene(registry, 4)
{
	ConstructEnvironment(registry);
	playerHandle = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));
	goalViewHandle = GoalViewFactory::CreateGoalViewEntity(registry, BMath::Vector3(20, 15, -60), 20, 0);

	parTextHandle = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<Behemoth::TextComponent>(parTextHandle, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));
}

void HoleTwoScene::Initialize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void HoleTwoScene::Update(const float deltaTime)
{
	Super::Update(deltaTime);

	if (Behemoth::Input::IsControllerKeyDown(Behemoth::CC_Y) || Behemoth::Input::IsKeyDown(Behemoth::KC_C))
	{
		ViewMode::ToggleViewMode(registry, playerHandle, goalViewHandle);
	}

	if (changeScene)
	{
		Behemoth::World::GetInstance().ChangeScene(new HoleThreeScene());
	}
}

void HoleTwoScene::OnEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void HoleTwoScene::Shutdown()
{

}

void HoleTwoScene::ConstructEnvironment(ECS::Registry& registry)
{
	environmentLightingHandle = registry.CreateEntity("Environment Lighting");
	Behemoth::DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLightingHandle);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
		directionalLight->intensity = 1.8f;
	}

	Behemoth::AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLightingHandle);
	if (ambientLight)
	{
		ambientLight->intensity = 0.8;
	}

	skySphereHandle = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky3.png", 999.0f, BMath::Vector2(1.0, 1.0));

	teeOffPlatformHandle = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 0.1f, 4));

	goalHandle = CreateGoalObject(registry, BMath::Vector3(20, 15, -60), BMath::Vector3(3.0f), 0.0f);
	goalComp = registry.GetComponent<GoalComponent>(goalHandle);

	for (int i = 0; i < 3; i++)
	{
		mainBarrierHandles[i] = BarrierFactory::CreateObstacle(
			registry,
			BMath::Vector3(-12 + (i * 20), 20, -30),
			BMath::Vector3(9, 15, 2), 
			BMath::Quaternion());
	}

	leftLongGrassPatchHandles[0] = PlatformFactory::CreateGrassPlatform(
		registry, 
		BMath::Vector3(-40, 8, 0),
		BMath::Vector3(15, 0.3f, 15), 
		BMath::Quaternion());

	leftLongGrassPatchHandles[1] = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(-40, 8, -50),
		BMath::Vector3(15, 0.3f, 15),
		BMath::Quaternion());

	leftLongGrassPatchHandles[2] = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(-40, 8, -90),
		BMath::Vector3(15, 0.3f, 15),
		BMath::Quaternion());

	grassPatch2Handle = PlatformFactory::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 8, -100),
		BMath::Vector3(25, 0.1f, 30),
		BMath::Quaternion());

	sandPatch2Handle = PlatformFactory::CreateSandPlatform(
		registry,
		BMath::Vector3(50, 8, -75),
		BMath::Vector3(20, 0.1f, 30), 
		BMath::Quaternion());

	barrierHandle = BarrierFactory::CreateObstacle(
		registry,
		BMath::Vector3(35, 20, -50),
		BMath::Vector3(5, 6, 10),
		BMath::Quaternion(DEGREE_TO_RAD(45.0f),
			BMath::Vector3(0, 1, 0)));
}