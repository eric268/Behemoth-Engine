#include "pch.h"
#include "MainScene.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"

#include <iostream>

MainScene::MainScene() : pointLight(registry.CreateNullEntity()), cube1(registry.CreateNullEntity()), cube2(registry.CreateNullEntity())
{

}

void MainScene::Init()
{
	InitSystems();
	
	Behemoth::CameraFactory cameraFactory{};
	ECS::Entity mainCameraEntity = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<CameraControllerComponent>(mainCameraEntity, 1.0f, 1.0f);

	Behemoth::DirectionalLightFactory dirLightFactory{};
	ECS::Entity dirLight = dirLightFactory.CreateDirectionalLight(registry);

	Behemoth::GameObjectFactory gameObjectFactory{};
	cube1 = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", "cube1");
	cube2 = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", "cube2");

	registry.AddComponent<Behemoth::MoveComponent>(cube1, Math::Vector3(-3.0f, 0.0f, -5.0f));
	registry.AddComponent<Behemoth::MoveComponent>(cube2, Math::Vector3(3.0f, 0.0f, -5.0f));

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

void MainScene::OnEvent(Behemoth::Event& e)
{
	Behemoth::EventDispatcher dispatcher{e};

	//  Maybe move this to world because in essentially any scene I would want this

	 dispatcher.Dispatch<Behemoth::KeyReleasedEvent>([&](Behemoth::KeyReleasedEvent keyEvent)
		 {
			 ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

			 if (cameraEntity.GetIdentifier() != NULL_ENTITY)
			 {
				 Behemoth::VelocityComponent* velocityComponent = registry.GetComponent<Behemoth::VelocityComponent>(cameraEntity);
				 if (velocityComponent)
				 {
					 velocityComponent->velocity = Math::Vector3::Zero();
				 }
			 }
		 });
}

void MainScene::Update(const float deltaTime)
{
	
}

void MainScene::Shutdown()
{

}

void MainScene::InitSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}