#include "MainScene.h"
#include "ECS/ECSCore.h"
#include "Factories/CameraFactory.h"
#include "Factories/LightFactories.h"
#include "Components/Components.h"
#include "Event/Event.h"
#include "Event/KeyboardEvents.h"
#include "Misc/CameraHelper.h"
#include "Systems/SystemManager.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"

#include <iostream>

MainScene::MainScene() : pointLight(registry.CreateNullEntity())
{

}

void MainScene::Init()
{
	InitSystems();
	
	Behemoth::CameraFactory cameraFactory{};
	ECS::Entity mainCameraEntity = cameraFactory.CreateCamera(registry, true, "Main Camera");
	//registry.AddComponent<CameraControllerComponent>(mainCameraEntity);

	Behemoth::DirectionalLightFactory dirLightFactory{};
	dirLightFactory.CreateDirectionalLight(registry);

	for (int i = -1; i < 2; i++)
	{
		ECS::Entity e1 = registry.CreateEntity("Cube 1");
		registry.AddComponent<Behemoth::MeshComponent>(e1, "monkey.obj", "diamond.png");
		registry.AddComponent<Behemoth::TransformComponent>(e1);
		registry.AddComponent<Behemoth::MeshInitalizeComponent>(e1);
		registry.AddComponent<Behemoth::RotationComponent>(e1, i + 2, 1.0f);
		registry.AddComponent<Behemoth::MoveComponent>(e1, Math::Vector3(-3.0f * i, 0.0f, -5.0f));
		registry.AddComponent<Behemoth::ScalingComponent>(e1, Math::Vector3(1.0f, 1.0f, 1.0f));

		registry.AddComponent<Behemoth::WireframeComponent>(e1, "cube.obj", true, Math::Vector3(0.0f, 1.0f, 0.0f));
		registry.AddComponent<Behemoth::BoundingVolumeComponent>(e1, 1.5f, false);
	}

	Behemoth::PointLightFactory pointLightFactory{};
	pointLight = pointLightFactory.CreatePointLight(registry, "Point Light 1");
	Behemoth::MoveComponent* pointLightMovementComp = registry.GetComponent<Behemoth::MoveComponent>(pointLight);
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
	Behemoth::EventDispatcher dispatcher{e};

	//  Maybe move this to world because in essentially any scene I would want this
	dispatcher.Dispatch<Behemoth::KeyDownEvent>([&](Behemoth::KeyDownEvent keyEvent)
		{
			ECS::Entity cameraEntity = Behemoth::CameraHelper::GetMainCameraEntity(registry);

			if (cameraEntity.GetIdentifier() != NULL_ENTITY)
			{
				Behemoth::VelocityComponent* velocityComponent = registry.GetComponent<Behemoth::VelocityComponent>(cameraEntity);

				auto [vel, transformComponent] = registry.GetMultipleComponents<Behemoth::VelocityComponent, Behemoth::TransformComponent>(cameraEntity);
				Behemoth::KeyCode keyCode = keyEvent.GetKeyCode();

				if (velocityComponent && transformComponent)
				{
					float movementSpeed = 1.0f;
					velocityComponent->velocity = Math::Vector3::Zero();

					if (keyCode == Behemoth::KeyCode::B_W)
					{
						 velocityComponent->velocity += transformComponent->forwardVector;
					}
					if (keyCode == Behemoth::KeyCode::B_A)
					{
						velocityComponent->velocity  -= transformComponent->rightVector;
					}
					if (keyCode == Behemoth::KeyCode::B_S)
					{
						velocityComponent->velocity  -= transformComponent->forwardVector;
					}
					if (keyCode == Behemoth::KeyCode::B_D)
					{
						velocityComponent->velocity  += transformComponent->rightVector;
					}
						
					velocityComponent->velocity.Normalize();
					velocityComponent->velocity *= movementSpeed;
				}

				if (keyCode == Behemoth::KeyCode::B_E)
				{
					Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(cameraEntity);
					if (rotationComponent)
					{
						rotationComponent->axis = Behemoth::RotationComponent::Y_AXIS;
						rotationComponent->speed = 0.5f;
					}
				}

				if (keyCode == Behemoth::KeyCode::B_Q)
				{
					Behemoth::RotationComponent* rotationComponent = registry.GetComponent<Behemoth::RotationComponent>(cameraEntity);
					if (rotationComponent)
					{
						rotationComponent->axis = Behemoth::RotationComponent::Y_AXIS;
						rotationComponent->speed = -0.5f;
					}
				}
			 }
	 	});

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