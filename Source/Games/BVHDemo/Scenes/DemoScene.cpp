#include "pch.h"
#include "DemoScene.h"

#include "Misc/GameObjectHelper.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"

#include "Factories/SkySphereFactory.h"

using namespace Behemoth;

DemoScene::DemoScene()
{
	mainCameraHandle = CameraFactory::CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 5));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<AmbientLightComponent>(environmentLighting, BMath::Vector3(0.7f), 0.5f);

	for (int i = 0; i < 5; i++)
	{
		demoCubes[i] = GameObjectFactory::CreateGameObject(registry, "cube.obj", "brick.png", "cube #" + std::to_string(i));
		registry.AddComponent<OBBColliderComponent>(demoCubes[i]);
		registry.AddComponent<StaticComponent>(demoCubes[i]);
	}

	registry.AddComponent<MoveComponent>(demoCubes[0], BMath::Vector3(-2.0f, 0.0f, -5.0f));
	registry.AddComponent<MoveComponent>(demoCubes[1], BMath::Vector3(2.0f, 0.0f, -5.0f));
	registry.AddComponent<MoveComponent>(demoCubes[2], BMath::Vector3(-1, 0, -6.7f));
	registry.AddComponent<MoveComponent>(demoCubes[3], BMath::Vector3(7, 1, -10.0f));
	registry.AddComponent<MoveComponent>(demoCubes[4], BMath::Vector3(10.5, -3, -6));
}

void DemoScene::Initialize()
{
	if (RotationComponent* rotationComponent = registry.GetComponent<RotationComponent>(demoCubes[4]))
	{
		BMath::Quaternion q(DEGREE_TO_RAD(45.0f), BMath::Vector3(0, 0, 1));
		rotationComponent->quat = q;
	}

	registry.AddComponent<ScalingComponent>(demoCubes[4], BMath::Vector3(2.5));
}

void DemoScene::OnEvent(Event& e)
{

}

void DemoScene::Update(const float deltaTime)
{
	if (Input::IsKeyReleased(KC_Space))
	{
 		World::GetInstance().GetActiveScene()->ReconstructStaticBVH();
	}

	Move(deltaTime, demoCubes[0], KC_I, KC_K, KC_L, KC_J);
	Move(deltaTime, mainCameraHandle, KC_W, KC_S, KC_D, KC_A);
}

void DemoScene::Shutdown()
{

}

void DemoScene::Move(float deltaTime, ECS::EntityHandle handle, KeyCode forward, KeyCode back, KeyCode right, KeyCode left)
{
	float moveSpeed = 3.0f;
	BMath::Vector3 moveVector = BMath::Vector3::Zero();

	if (Input::IsKeyHeld(forward))
	{
		moveVector += BMath::Vector3::Forward() * moveSpeed;
	}

	if (Input::IsKeyHeld(back))
	{
		moveVector -= BMath::Vector3::Forward() * moveSpeed;
	}

	if (Input::IsKeyHeld(right))
	{
		moveVector += BMath::Vector3::Right() * moveSpeed;
	}

	if (Input::IsKeyHeld(left))
	{
		moveVector -= BMath::Vector3::Right() * moveSpeed;
	}

	if (!(BMath::Vector3::Equals(moveVector, BMath::Vector3::Zero())))
	{
		registry.AddComponent<MoveComponent>(handle, moveVector * deltaTime);
	}
}