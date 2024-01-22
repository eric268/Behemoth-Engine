#include "pch.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"

#include "Factories/SkySphereFactory.h"

MainScene::MainScene()
{
	mainCameraHandle = Behemoth::CameraFactory::CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 0));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky3.png", { 1.0, 1.0 });

	exampleParentEntity = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png", "Example Parent");
	registry.AddComponent<Behemoth::MoveComponent>(exampleParentEntity, BMath::Vector3(0.0f, 0.0f, -5.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleParentEntity);
	registry.AddComponent<Behemoth::ScalingComponent>(exampleParentEntity, BMath::Vector3(0.5f));
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleParentEntity, false, true);



	exampleChildEntity1 = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "brick.png", "Child 1");
	registry.AddComponent<Behemoth::MoveComponent>(exampleChildEntity1, BMath::Vector3(-2.0f, 0.0f, 0.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleChildEntity1);
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleChildEntity1, false, true);
	Behemoth::GameObjectFactory::AddChildObject(registry, exampleParentEntity, exampleChildEntity1);

	exampleChildEntity2 = Behemoth::GameObjectFactory::CreateGameObject(registry, "sphere.obj", "brick.png", "Child 2");
	registry.AddComponent<Behemoth::MoveComponent>(exampleChildEntity2, BMath::Vector3(2.0f, 0.0f, 0.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleChildEntity2);
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleChildEntity2, false, true);
	Behemoth::GameObjectFactory::AddChildObject(registry, exampleParentEntity, exampleChildEntity2);


	LOGMESSAGE(General, "Main Scene constructed\n");
}

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void MainScene::OnEvent(Behemoth::Event& e)
{

}

void MainScene::Update(const float deltaTime)
{
	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(exampleParentEntity))
	{
		parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(1.5f), BMath::Vector3(0, 1, 0));
	}

	if (Behemoth::Input::IsKeyDown(Behemoth::KeyCode::KC_Space))
	{
		registry.AddComponent<Behemoth::ScalingComponent>(exampleParentEntity, BMath::Vector3(0.25f));
	}
}

void MainScene::Shutdown()
{

}