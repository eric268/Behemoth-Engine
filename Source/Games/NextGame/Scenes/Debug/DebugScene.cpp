#include "pch.h"
#include "DebugScene.h"
#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Factories/SkySphereFactory.h"

DebugScene::DebugScene()
{
	mainCameraHandle = Behemoth::CameraFactory::CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 10, 0));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "seamlesssky3.png", { 1.0, 1.0 });

	exampleParentEntity = Behemoth::GameObjectFactory::CreateGameObject(registry, "sphere.obj", "rock.png", "Example Parent");
	registry.AddComponent<Behemoth::MoveComponent>(exampleParentEntity, BMath::Vector3(0.0f, 10.0f, -10.0f));
	registry.AddComponent<Behemoth::SphereColliderComponent>(exampleParentEntity);
	registry.AddComponent<Behemoth::ScalingComponent>(exampleParentEntity, BMath::Vector3(1.0f));
	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleParentEntity, false, true);
	registry.AddComponent<Behemoth::WireframeComponent>(exampleParentEntity, "sphere.obj");


	exampleChildEntity1 = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "brick.png", "Child 1");
	registry.AddComponent<Behemoth::MoveComponent>(exampleChildEntity1, BMath::Vector3(0.0f, 0.0f, -15.0f));
	registry.AddComponent<Behemoth::OBBColliderComponent>(exampleChildEntity1);
	registry.AddComponent<Behemoth::StaticComponent>(exampleChildEntity1);
//	registry.AddComponent<Behemoth::RigidBodyComponent>(exampleChildEntity1, false, true);
	registry.AddComponent<Behemoth::ScalingComponent>(exampleChildEntity1, BMath::Vector3(1.0f, 5.25, 5.0f));
	registry.AddComponent<Behemoth::RotationComponent>(exampleChildEntity1, BMath::Quaternion(DEGREE_TO_RAD(45.0f), BMath::Vector3(0.0f, 1.0, 0.0f)), false);

	LOGMESSAGE(General, "Main Scene constructed\n");
}

void DebugScene::Initialize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction

}

void DebugScene::OnEvent(Behemoth::Event& e)
{

}

void DebugScene::Update(const float deltaTime)
{
	MoveObject(
		mainCameraHandle,
		Behemoth::KC_W,
		Behemoth::KC_S,
		Behemoth::KC_A,
		Behemoth::KC_D, 
		Behemoth::KC_E,
		Behemoth::KC_Q,
		Behemoth::KC_C,
		Behemoth::KC_Z);

	MoveObject(
		exampleParentEntity, 
		Behemoth::KC_I, 
		Behemoth::KC_K,
		Behemoth::KC_J,
		Behemoth::KC_L,
		Behemoth::KC_O,
		Behemoth::KC_U,
		Behemoth::KC_Plus,
		Behemoth::KC_Minus);
}

void DebugScene::Shutdown()
{

}

void DebugScene::MoveObject(
	ECS::EntityHandle handle,
	Behemoth::KeyCode forward,
	Behemoth::KeyCode back,
	Behemoth::KeyCode left,
	Behemoth::KeyCode right,
	Behemoth::KeyCode up,
	Behemoth::KeyCode down,
	Behemoth::KeyCode rotLeft,
	Behemoth::KeyCode rotRight)
{
	BMath::Vector3 move;

	if (Behemoth::Input::IsKeyHeld(forward))
	{
		move += BMath::Vector3::Forward();
	}
	if (Behemoth::Input::IsKeyHeld(back))
	{
		move -= BMath::Vector3::Forward();
	}
	if (Behemoth::Input::IsKeyHeld(right))
	{
		move += BMath::Vector3::Right();
	}
	if (Behemoth::Input::IsKeyHeld(left))
	{
		move -= BMath::Vector3::Right();
	}
	if (Behemoth::Input::IsKeyHeld(up))
	{
		move += BMath::Vector3::Up();
	}
	if (Behemoth::Input::IsKeyHeld(down))
	{
		move -= BMath::Vector3::Up();
	}

	float movementSpeed = 0.05f;
	registry.AddComponent<Behemoth::MoveComponent>(handle, move * movementSpeed);

	float rot = 0.0f;
	if (Behemoth::Input::IsKeyHeld(rotRight))
	{
		rot += 2.5f;
	}
	if (Behemoth::Input::IsKeyHeld(rotLeft))
	{
		rot -= 2.5f;
	}
	registry.AddComponent <Behemoth::RotationComponent>(handle, BMath::Quaternion(DEGREE_TO_RAD(rot), BMath::Vector3(0, 1, 0)));
}
