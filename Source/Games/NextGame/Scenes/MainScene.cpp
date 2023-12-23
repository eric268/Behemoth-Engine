#include "MainScene.h"
#include "Factories/CameraFactory.h"
#include "Factories/LightFactories.h"
#include "Components/Components.h"
#include "Event/Event.h"
#include <iostream>

MainScene::MainScene() : pointLight(registry.CreateNullEntity())
{

}

void MainScene::Init()
{
	Behemoth::CameraFactory cameraFactory{};
	cameraFactory.CreateCamera(registry, true, "Main Camera");

	Behemoth::DirectionalLightFactory dirLightFactory{};
	dirLightFactory.CreateDirectionalLight(registry);


	for (int i = -1; i < 2; i++)
	{
		ECS::Entity e1 = registry.CreateEntity("Cube 1");
		registry.AddComponent<Behemoth::MeshComponent>(e1, "monkey.obj", "diamond.png");
		registry.AddComponent<Behemoth::TransformComponent>(e1);
		registry.AddComponent<Behemoth::MeshInitalizeComponent>(e1);
		registry.AddComponent<Behemoth::RotationComponent>(e1, i + 1, 1.0f);
		registry.AddComponent<Behemoth::MovementComponent>(e1, Math::Vector3(-3.0f * i, 0.0f, -5.0f));
		registry.AddComponent<Behemoth::ScalingComponent>(e1, Math::Vector3(1.0f, 1.0f, 1.0f));

		registry.AddComponent<Behemoth::BoundingVolumeComponent>(e1, 1.5f, false);
	}

	Behemoth::PointLightFactory pointLightFactory{};
	pointLight = pointLightFactory.CreatePointLight(registry, "Point Light 1");
	Behemoth::MovementComponent* pointLightMovementComp = registry.GetComponent<Behemoth::MovementComponent>(pointLight);
	if (pointLightMovementComp)
		pointLightMovementComp->location = Math::Vector3(-3.0f, 2, -3.0f);

	Behemoth::PointLightComponent* pointLightComponent = registry.GetComponent<Behemoth::PointLightComponent>(pointLight);
	if (pointLightComponent)
	{
		pointLightComponent->intensity = 2.0f;
	}
}

void MainScene::OnEvent(Behemoth::Event& e)
{
	std::cout << "Event: " << e.GetEventName() << " received in MainScene\n";
}

void MainScene::Update(const float deltaTime)
{

}

void MainScene::Shutdown()
{

}
