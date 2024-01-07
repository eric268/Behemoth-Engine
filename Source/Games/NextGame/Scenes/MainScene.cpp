#include "pch.h"
#include "MainScene.h"

#include "Physics/Collision/BroadCollision.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"


MainScene::MainScene()
{
	std::cout << "Main Scene constructed\n";

	Behemoth::CameraFactory cameraFactory{};
	ECS::EntityHandle mainCameraEntity = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<CameraControllerComponent>(mainCameraEntity, 5.0f, 1.0f, true, Behemoth::KeyCode::KC_Up, Behemoth::KeyCode::KC_Down, Behemoth::KeyCode::KC_Left, Behemoth::KeyCode::KC_Right, Behemoth::KeyCode::KC_Plus, Behemoth::KeyCode::KC_Minus);
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraEntity, BMath::Vector3(0, 0, 5));

	Behemoth::DirectionalLightFactory dirLightFactory{};
	ECS::EntityHandle dirLight = dirLightFactory.CreateDirectionalLight(registry);

	Behemoth::GameObjectFactory gameObjectFactory{};

// 	for (int i = 0; i < 4; i++)
// 	{
// 		std::string name = "Cube " + std::to_string(i + 1);
// 		cubes[i] = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", name);
// 		registry.AddComponent<Behemoth::AABBColliderComponent>(cubes[i], BMath::Vector3(1.5f, 1.0, 0.5f));
// 		registry.AddComponent<Behemoth::ScalingComponent>(cubes[i], BMath::Vector3(1.5f, 1.0, 0.5f));
// 		registry.AddComponent<Behemoth::WireframeComponent>(cubes[i], "cube.obj", BMath::Vector3(1.5f, 1.0, 0.5f), false, BMath::Vector3(0.0f, 1.0f, 0.0f));
// 		registry.AddComponent<Behemoth::StaticComponent>(cubes[i]);
// 	}
// 
// 	for (int i = 4; i < 8; i++)
// 	{
// 		std::string name = "Cube " + std::to_string(i + 1);
// 		cubes[i] = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", name);
// 		registry.AddComponent<Behemoth::AABBColliderComponent>(cubes[i], BMath::Vector3(1.5f, 1.0, 0.5f));
// 		registry.AddComponent<Behemoth::ScalingComponent>(cubes[i], BMath::Vector3(1.5f, 1.0, 0.5f));
// 		registry.AddComponent<Behemoth::WireframeComponent>(cubes[i], "cube.obj", BMath::Vector3(1.5f, 1.0, 0.5f), false, BMath::Vector3(0.0f, 1.0f, 0.0f));
// 		registry.AddComponent<Behemoth::StaticComponent>(cubes[i]);
// 	}
// 
// 	// static 
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[0], BMath::Vector3(-8.0f, -5.0f, -10.0f));
//  	registry.AddComponent<Behemoth::MoveComponent>(cubes[1], BMath::Vector3(-11.0f, 3.0f, -7.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[2], BMath::Vector3(-14.0f, -4.0f, -3.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[3], BMath::Vector3(-10.0f, 4.0f, -12.0f));
// 
// 	//dynamic
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[4], BMath::Vector3( 8.0f, -5.0f, -5.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[5], BMath::Vector3( 11.0f, 0.0f, -12.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[6], BMath::Vector3( 14.0f, -3.0f, -6.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[7], BMath::Vector3( 10.0f, 3.0f, -9.0f));


	playerHandle = gameObjectFactory.CreateGameObject(registry, "cube.obj", "brick.png", "Player");
	registry.AddComponent<Behemoth::AABBColliderComponent>(playerHandle, 1.0f);
	registry.AddComponent<Behemoth::WireframeComponent>(playerHandle, "cube.obj", BMath::Vector3(1.0f), false, BMath::Vector3(0.0f, 1.0f, 0.0f));
	registry.AddComponent<CameraControllerComponent>(playerHandle, 3.0f, 1.0f, false, Behemoth::KeyCode::KC_W, Behemoth::KeyCode::KC_S, Behemoth::KeyCode::KC_A, Behemoth::KeyCode::KC_D, Behemoth::KeyCode::KC_E, Behemoth::KeyCode::KC_Q);
	registry.AddComponent<Behemoth::MoveComponent>(playerHandle, BMath::Vector3(0.0f, 0.0f, -5.0f));
	registry.AddComponent<Behemoth::RigidBodyComponent>(playerHandle, false);


	Behemoth::PointLightFactory pointLightFactory{};
	pointLight = pointLightFactory.CreatePointLight(registry, "Point Light 1");
	Behemoth::MoveComponent* pointLightMovementComp = registry.GetComponent<Behemoth::MoveComponent>(pointLight);
	if (pointLightMovementComp)
		pointLightMovementComp->location = BMath::Vector3(0.0f, 0, 0.0f);

	Behemoth::PointLightComponent* pointLightComponent = registry.GetComponent<Behemoth::PointLightComponent>(pointLight);
	if (pointLightComponent)
	{
		pointLightComponent->intensity = 2.0f;
	}

}

void MainScene::Initalize()
{
	InitSystems();
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
		Behemoth::TransformComponent* cameraTransform = Behemoth::CameraHelper::GetMainCameraTransform(registry);
		float distance = 50.0f;
		BMath::Vector3 startPos = cameraTransform->position + cameraTransform->forwardVector * 0.5f;
		BMath::Vector3 endPos = cameraTransform->position + cameraTransform->forwardVector * distance;

		ECS::EntityHandle debugLineHandle = registry.CreateEntity("Debug line");
		registry.AddComponent<Behemoth::DebugLineComponent>(debugLineHandle, startPos, endPos, 20.0f);
	}

	if (auto comp = registry.GetComponent<Behemoth::RotationComponent>(playerHandle))
	{
		BMath::Vector3 rot;
		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_T))
		{
			rot.x = 1.0f;
		}
		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Y))
		{
			rot.y = 1.0f;
		}
		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_U))
		{
			rot.z = 1.0f;
		}
		comp->quat = BMath::Quaternion(DEGREE_TO_RAD(2.5f), rot);
	}
}

void MainScene::Shutdown()
{

}

void MainScene::InitSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}