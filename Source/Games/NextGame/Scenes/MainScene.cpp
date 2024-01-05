#include "pch.h"
#include "MainScene.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"

#include <iostream>

MainScene::MainScene()
{
	std::cout << "Main Scene constructed\n";

	Behemoth::CameraFactory cameraFactory{};
	ECS::EntityHandle mainCameraEntity = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<CameraControllerComponent>(mainCameraEntity, 1.0f, 1.0f, true, Behemoth::KeyCode::KC_Up, Behemoth::KeyCode::KC_Down, Behemoth::KeyCode::KC_Left, Behemoth::KeyCode::KC_Right);

	Behemoth::DirectionalLightFactory dirLightFactory{};
	ECS::EntityHandle dirLight = dirLightFactory.CreateDirectionalLight(registry);

	Behemoth::GameObjectFactory gameObjectFactory{};

	for (int i = 0; i < 8; i++)
	{
		cubes[i] = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", "cube1");
		registry.AddComponent<Behemoth::AABBColliderComponent>(cubes[i], Math::Vector3(1.5, 1.75, 1.0f));
		registry.AddComponent<Behemoth::ScalingComponent>(cubes[i], Math::Vector3(1.5, 1.75, 1.0f));
		registry.AddComponent<Behemoth::WireframeComponent>(cubes[i], "cube.obj", Math::Vector3(1.5, 1.75, 1.0f), false, Math::Vector3(0.0f, 1.0f, 0.0f));
	}

	registry.AddComponent<CameraControllerComponent>(cubes[0], 1.0f, 1.0f, true, Behemoth::KeyCode::KC_W, Behemoth::KeyCode::KC_S, Behemoth::KeyCode::KC_A, Behemoth::KeyCode::KC_D);

	registry.AddComponent<Behemoth::MoveComponent>(cubes[0], Math::Vector3(-3.0f, -1.0f, -8.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[1], Math::Vector3(5.0f, 0.0f, -7.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[2], Math::Vector3(-3.0f, -3.0f, -6.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[3], Math::Vector3(1.0f, 3.0f, -9.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[4], Math::Vector3(0.0f, -6.0f, 0.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[5], Math::Vector3(0.0f, 0.0f, 1.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[6], Math::Vector3(-8.0f, -3.0f, -6.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cubes[7], Math::Vector3(8.0f, 3.0f, -9.0f));


	Behemoth::PointLightFactory pointLightFactory{};
	pointLight = pointLightFactory.CreatePointLight(registry, "Point Light 1");
	Behemoth::MoveComponent* pointLightMovementComp = registry.GetComponent<Behemoth::MoveComponent>(pointLight);
	if (pointLightMovementComp)
		pointLightMovementComp->location = Math::Vector3(0.0f, 0, -3.0f);

	Behemoth::PointLightComponent* pointLightComponent = registry.GetComponent<Behemoth::PointLightComponent>(pointLight);
	if (pointLightComponent)
	{
		pointLightComponent->intensity = 2.0f;
	}

}

void MainScene::Init()
{
	InitSystems();
	collisionBVH.OnConstruction(registry);
}

void MainScene::OnEvent(Behemoth::Event& e)
{
	Behemoth::EventDispatcher dispatcher{e};

	//  Maybe move this to world because in essentially any scene I would want this

	 dispatcher.Dispatch<Behemoth::WindowResizeEvent>([&](Behemoth::WindowResizeEvent keyEvent)
		 {
			 ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

			 if (cameraEntity.GetIdentifier() != NULL_ENTITY)
			 {
				 Behemoth::CameraComponent* cameraComponent = registry.GetComponent<Behemoth::CameraComponent>(cameraEntity);
				 if (cameraComponent)
				 {
					 cameraComponent->isDirty = true;
				 }
			 }
		 });
}

void MainScene::Update(const float deltaTime)
{
	if (Behemoth::Input::IsKeyReleased(Behemoth::KeyCode::KC_Space))
	{
		collisionBVH.OnReconstruction(registry);
	}
}

void MainScene::Shutdown()
{

}

void MainScene::InitSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}