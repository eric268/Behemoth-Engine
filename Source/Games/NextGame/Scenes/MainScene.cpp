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

#include "TestScene.h"

using namespace Behemoth;

MainScene::MainScene()
{
	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 5));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "brick.png", { 8.0, 8.0 });

  	GameObjectFactory gameObjectFactory{};
	exampleParentEntity = gameObjectFactory.CreateGameObject(registry, "monkey.obj", "rock.png", "Example Parent");
	registry.AddComponent<MoveComponent>(exampleParentEntity, BMath::Vector3(0.0f, 0.0f, -5.0f));
	registry.AddComponent<OBBColliderComponent>(exampleParentEntity);
	registry.AddComponent<RigidBodyComponent>(exampleParentEntity, false);
	registry.AddComponent<CameraControllerComponent>(exampleParentEntity, 5.0f, 0.33f, false, Behemoth::KeyCode::KC_W, Behemoth::KeyCode::KC_S, Behemoth::KeyCode::KC_A, Behemoth::KeyCode::KC_D, Behemoth::KeyCode::KC_E, Behemoth::KeyCode::KC_Q);
	registry.AddComponent<ScalingComponent>(exampleParentEntity, BMath::Vector3(0.5f));

	exampleChildEntity1 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "cube.obj", "brick.png", "Child 1");
	registry.AddComponent<MoveComponent>(exampleChildEntity1, BMath::Vector3(-5.0f, 0.0f, 0.0f));
	registry.AddComponent<RigidBodyComponent>(exampleChildEntity1, false);

	exampleChildEntity2 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "sphere.obj", "brick.png", "Child 2");
	registry.AddComponent<MoveComponent>(exampleChildEntity2, BMath::Vector3(5.0f, 0.0f, 0.0f));
	registry.AddComponent<RigidBodyComponent>(exampleChildEntity2, false);

	Behemoth::LightFactory lightFactory{};
	pointLight = lightFactory.CreatePointLight(registry);
	registry.AddComponent<Behemoth::MoveComponent>(pointLight, BMath::Vector3(0.0f,0.0f,-2.0f));

	LOGMESSAGE(General, "Main Scene constructed\n");
 }

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
	SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}

void MainScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void MainScene::Update(const float deltaTime)
{
	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(exampleParentEntity))
	{
		parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(1.5f), BMath::Vector3(0, 1, 0));
	}
}

void MainScene::Shutdown()
{

}