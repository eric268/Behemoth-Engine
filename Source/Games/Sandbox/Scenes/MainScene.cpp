#include "MainScene.h"
#include "Factories/CameraFactory.h"
#include "Factories/LightFactories.h"
#include "Components/Components.h"

//stl
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

	Behemoth::PointLightFactory pointLightFactory{};
	pointLight = pointLightFactory.CreatePointLight(registry, "Point Light 1");

	for (int i = -1; i < 2; i++)
	{
		ECS::Entity entity = registry.CreateEntity("Monkey " + std::to_string(i));
		registry.AddComponent<Behemoth::MeshComponent>(entity, "monkey.obj", "diamond.png");
		registry.AddComponent<Behemoth::TransformComponent>(entity);
		registry.AddComponent<Behemoth::MeshInitalizeComponent>(entity);
		registry.AddComponent<Behemoth::RotationComponent>(entity, i + 1, 1.0f);
		registry.AddComponent<Behemoth::MovementComponent>(entity, Math::Vector3(-3.0f * i, 0.0f, -5.0f));
		registry.AddComponent<Behemoth::ScalingComponent>(entity, Math::Vector3(1.0f, 1.0f, 1.0f));
		registry.AddComponent<Behemoth::BoundingVolumeComponent>(entity, 1.5f, false);
	}
}

void MainScene::Update(const float deltaTime)
{

}

void MainScene::Shutdown()
{

}