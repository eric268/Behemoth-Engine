#include "pch.h"
#include "TestScene.h"
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

TestScene::TestScene()
{
	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<CameraControllerComponent>(mainCameraHandle, 5.0f, 0.33f, false, Behemoth::KeyCode::KC_W, Behemoth::KeyCode::KC_S, Behemoth::KeyCode::KC_A, Behemoth::KeyCode::KC_D, Behemoth::KeyCode::KC_E, Behemoth::KeyCode::KC_Q);
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 0));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "rock.png", { 8.0, 8.0 });

	Behemoth::GameObjectFactory gameObjectFactory{};
	exampleParentEntity = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "rock.png", "Example Parent");
	registry.AddComponent<Behemoth::MoveComponent>(exampleParentEntity, BMath::Vector3(0.0f, 0.0f, -5.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleParentEntity);
	registry.AddComponent<Behemoth::ScalingComponent>(exampleParentEntity, BMath::Vector3(0.5f));
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleParentEntity, false);

	exampleChildEntity1 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "monkey.obj", "brick.png", "Child 1");
	registry.AddComponent<Behemoth::MoveComponent>(exampleChildEntity1, BMath::Vector3(-2.0f, 0.0f, 0.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleChildEntity1);
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleChildEntity1, false);

	exampleChildEntity2 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "monkey.obj", "brick.png", "Child 2");
	registry.AddComponent<Behemoth::MoveComponent>(exampleChildEntity2, BMath::Vector3(2.0f, 0.0f, 0.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleChildEntity2);
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleChildEntity2, false);

	Behemoth::LightFactory lightFactory{};
	pointLight = lightFactory.CreatePointLight(registry);
	registry.AddComponent<Behemoth::MoveComponent>(pointLight, BMath::Vector3(0.0f, 0.0f, -2.0f));

	LOGMESSAGE(General, "Main Scene constructed\n");
}

void TestScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void TestScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void TestScene::Update(const float deltaTime)
{
	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(exampleParentEntity))
	{
		parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(1.5f), BMath::Vector3(0, 0, 1));
	}

	if (Behemoth::Input::IsKeyDown(Behemoth::KeyCode::KC_Space))
	{
		Behemoth::Scene* mainScene = new MainScene();
		Behemoth::World::GetInstance().ChangeScene(mainScene);
	}
}

void TestScene::Shutdown()
{

}