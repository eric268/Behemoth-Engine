#include "pch.h"
#include "DemoScene.h"

#include "Misc/GameObjectHelper.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"

#include "Factories/SkySphereFactory.h"

DemoScene::DemoScene()
{
	mainCameraHandle = Behemoth::CameraFactory::CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 0));

	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting, BMath::Vector3(0.7f), 0.5f);

	skySphereHandle = Behemoth::SkySphereFactory::CreateSkySphere(registry, "sky.jpg", 999.0f, { 1.0f, 1.0f });


	LOGMESSAGE(General, "Demo Scene constructed\n");
}

void DemoScene::Initialize()
{
	parentHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png");
	childHandle1 = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png");
	childHandle2 = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png");

	Behemoth::GameObjectHelper::AddChildObject(registry, parentHandle, childHandle1);
	Behemoth::GameObjectHelper::AddChildObject(registry, parentHandle, childHandle2);

	registry.AddComponent<Behemoth::MoveComponent>(parentHandle, BMath::Vector3(0.0f, 0.0f, -10.0f));
	registry.AddComponent<Behemoth::MoveComponent>(childHandle1, BMath::Vector3(-3.0f, 0.0f, 0.0));
	registry.AddComponent<Behemoth::MoveComponent>(childHandle2, BMath::Vector3(3.0f, 0.0f, 0.0));
}

void DemoScene::OnEvent(Behemoth::Event& e)
{

}

void DemoScene::Update(const float deltaTime)
{
	if (Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(parentHandle))
	{
		BMath::Quaternion q(DEGREE_TO_RAD(1.0f), BMath::Vector3(0, 1, 0));
		rotationComponent->quat = q;
	}

	if (Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(childHandle1))
	{
		BMath::Quaternion q(DEGREE_TO_RAD(1.0f), BMath::Vector3(1, 0, 0));
		rotationComponent->quat = q;
	}

	if (Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(childHandle2))
	{
		BMath::Quaternion q(DEGREE_TO_RAD(1.0f), BMath::Vector3(0, 0, 1));
		rotationComponent->quat = q;
	}
}

void DemoScene::Shutdown()
{

}