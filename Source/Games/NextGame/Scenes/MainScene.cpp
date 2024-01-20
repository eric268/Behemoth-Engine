#include "pch.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Components/AudioComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"
#include "Components/UIComponents.h"

#include "Factories/SkySphereFactory.h"

#include "Physics/ProjectileMotion.h"

#include "GameSystems/PCSystem.h"

#include "TestScene.h"

#include "Scripts/GameObjects/PlayerFactory.h"

using namespace Behemoth;

MainScene::MainScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 10));

	GameObjectFactory gameObjectFactory{};
	obstacleHandle = gameObjectFactory.CreateGameObject(registry, "cube.obj", "brick.png");
	registry.AddComponent<ScalingComponent>(obstacleHandle, BMath::Vector3(3, 3, 0.1));
	registry.AddComponent<MoveComponent>(obstacleHandle, BMath::Vector3(0, 10, 0));
	registry.AddComponent<OBBColliderComponent>(obstacleHandle);
	registry.AddComponent<StaticComponent>(obstacleHandle);

	

	groundEntity = gameObjectFactory.CreateGameObject(registry, "plane.obj", "brick.png", "Ground entity", { 8,8 });
	registry.AddComponent<MoveComponent>(groundEntity, BMath::Vector3(0, 0, 0));
	registry.AddComponent<ScalingComponent>(groundEntity, BMath::Vector3(10, 1.1f, 10));
	registry.AddComponent<OBBColliderComponent>(groundEntity, BMath::Vector3(1.0f));
	registry.AddComponent<StaticComponent>(groundEntity);
	registry.AddComponent<RotationComponent>(groundEntity, BMath::Quaternion(DEGREE_TO_RAD(1), BMath::Vector3(1,0,0)));
	// registry.AddComponent<WireframeComponent>(groundEntity, "plane.obj", BMath::Vector3(1, 0.1, 1));

}

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
	InitalizeSystems();
}

void MainScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void MainScene::Update(const float deltaTime)
{

}

void MainScene::InitalizeSystems()
{
	SystemManager::GetInstance().AddSystem<PCSystem>();
	SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}

void MainScene::Shutdown()
{

}