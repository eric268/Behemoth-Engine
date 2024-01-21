#include "pch.h"
#include "HoleOneScene.h"
#include "HoleTwoScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Components/AudioComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "Components/UIComponents.h"

#include "Factories/SkySphereFactory.h"

#include "Physics/ProjectileMotion.h"

#include "GameSystems/PCSystem.h"
#include "Physics/Collision/PhysicsMaterial.h"



#include "Scripts/PlayerFactory.h"
#include "Scripts/LevelViewFactory.h"
#include "Scripts/ViewModeChange.h"
#include "Scripts/GoalObject.h"
#include "Scripts/PlatformObject.h"
#include "Scripts/BarrierObject.h"
#include "Scripts/PlayerScore.h"

#include "GameComponents/Player/PlayerComponent.h"
#include "GameComponents/Level/LevelViewComponent.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"

using namespace Behemoth;

HoleOneScene::HoleOneScene()
{
	PlayerScore::ResetScore();

	environmentLighting = registry.CreateEntity("Environment Lighting");
	DirectionalLightComponent* directionalLight = registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	if (directionalLight)
	{
		directionalLight->direction = BMath::Vector3(0.0f, 0.707, 0.707);
	}
	AmbientLightComponent* ambientLight = registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	if (ambientLight)
	{
		ambientLight->intensity = 2;
	}
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	goalObject = GoalObject::CreateGoal(registry, BMath::Vector3(0, 2, -20), BMath::Vector3(3.0f), 45.0f);

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 18));

	teeOffPlatform = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0, 9, 18),
		BMath::Vector3(4, 1.0f, 4));

	obstacleHandle = BarrierObject::CreateObstacle(registry, BMath::Vector3(0, 5, -12), BMath::Vector3(3.0f, 3.0f, 1.0f), BMath::Quaternion(), false);
	registry.AddComponent<MovingObsComponent>(obstacleHandle, BMath::Vector3::Up(), 30.0f, 250.0f, 0.0f);
	registry.AddComponent<Behemoth::VelocityComponent>(obstacleHandle);
	registry.AddComponent<Behemoth::RigidBodyComponent>(obstacleHandle);

	grassEntity = PlatformObject::CreateGrassPlatform(
		registry,
		BMath::Vector3(0,0,0),
		BMath::Vector3(10, 1.0f, 6));

	sandTrap1 = PlatformObject::CreateSandPlatform(
		registry,
		BMath::Vector3(-20, -2, -20),
		BMath::Vector3(7, 1.0f, 10));

	sandTrap2 = PlatformObject::CreateSandPlatform(
		registry,
		BMath::Vector3(20, -2, -20),
		BMath::Vector3(7, 1.0f, 10));


	bottomOOBTrigger = GameObjectFactory::CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });
	registry.AddComponent<OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<ScalingComponent>(bottomOOBTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -20, 10.0f));

	if (MeshComponent* mesh = registry.GetComponent<MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	LevelViewFactory levelViewFactory{};
	levelViewEntity = levelViewFactory.CreateLevelViewEntity(registry, BMath::Vector3(0, 2, -20), 5, 20, 10, 0);
	registry.AddComponent<TransformComponent>(levelViewEntity);
	registry.AddComponent<RotationComponent>(levelViewEntity);

	par = 2;
	parTextEntity = registry.CreateEntity("Par Text Entity");
	registry.AddComponent<TextComponent>(parTextEntity, "Par: " + std::to_string(par), BMath::Vector2(0.85f, 0.7f));

	delayUntilSceneChange = 3.0f;
	changeScene = false;
}

void HoleOneScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void HoleOneScene::OnEvent(Behemoth::Event& e)
{
}

void HoleOneScene::Update(const float deltaTime)
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

	if (Input::IsControllerKeyDown(CC_Y) || Input::IsKeyDown(KC_C))
	{
		ViewModeChange::ChangeViewMode(registry, playerCharacter, levelViewEntity);
	}

	if (changeScene)
	{
		World::GetInstance().ChangeScene(new HoleTwoScene());
	}
}

void HoleOneScene::Shutdown()
{

}