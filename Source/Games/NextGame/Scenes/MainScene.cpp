#include "pch.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"


MainScene::MainScene()
{
	std::cout << "Main Scene constructed\n";

	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<CameraControllerComponent>(mainCameraHandle, 5.0f, 0.33f, false, Behemoth::KeyCode::KC_W, Behemoth::KeyCode::KC_S, Behemoth::KeyCode::KC_A, Behemoth::KeyCode::KC_D, Behemoth::KeyCode::KC_E, Behemoth::KeyCode::KC_Q);
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 3, 10));

	Behemoth::LightFactory lightFactory{};
	ECS::EntityHandle environmentLighting = lightFactory.CreateDirectionalLight(registry);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
//  
  	Behemoth::GameObjectFactory gameObjectFactory{};

	ECS::EntityHandle cubemapHandle = registry.CreateEntity("Cubemap");
	registry.AddComponent<Behemoth::SkySphereComponent>(cubemapHandle, "SegmentSphere15.obj", "Sky.jpg", BMath::Vector2(1.0f, 1.0f), true);
	registry.AddComponent<Behemoth::TransformComponent>(cubemapHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(cubemapHandle, BMath::Vector3(900.0f));
	registry.AddComponent<Behemoth::RotationComponent>(cubemapHandle, BMath::Quaternion(DEGREE_TO_RAD(180), BMath::Vector3(0,0,1)));


// 
// 	for (int i = 0; i < 4; i++)
// 	{
// 		std::string name = "Cube " + std::to_string(i + 1);
// 		cubes[i] = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", name);
// 		registry.AddComponent<Behemoth::ScalingComponent>(cubes[i], BMath::Vector3(1.f, 1.0, 1.f));
// 		registry.AddComponent<Behemoth::StaticComponent>(cubes[i]);
// 		registry.AddComponent<Behemoth::OBBColliderComponent>(cubes[i]);
// 	}
// 
//  for (int i = 4; i < 8; i++)
//  {
//  	std::string name = "Cube " + std::to_string(i + 1);
//  	cubes[i] = gameObjectFactory.CreateGameObject(registry, "cube.obj", "rock.png", name);
//  	registry.AddComponent<Behemoth::AABBColliderComponent>(cubes[i], BMath::Vector3(1.5f, 1.0, 0.5f));
//  	registry.AddComponent<Behemoth::ScalingComponent>(cubes[i], BMath::Vector3(2.f, 1.0, 1.f));
//  	registry.AddComponent<Behemoth::StaticComponent>(cubes[i]);
//  	registry.AddComponent<Behemoth::OBBColliderComponent>(cubes[i]);
//  }
// 
// 	// static 
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[0], BMath::Vector3(-2.0f, 0.0f, 0.0f));
// 	registry.AddComponent<Behemoth::MoveComponent>(cubes[1], BMath::Vector3(2.0f, 0.0f, 0.0f));
//     registry.AddComponent<Behemoth::MoveComponent>(cubes[2], BMath::Vector3(0.0f, 3.0f, 0.0f));
//     registry.AddComponent<Behemoth::MoveComponent>(cubes[3], BMath::Vector3(0.0f, -3.0f, 0.0f));
// //  
//  	//dynamic
//  	registry.AddComponent<Behemoth::MoveComponent>(cubes[4], BMath::Vector3( 8.0f, -5.0f, -5.0f));
//  	registry.AddComponent<Behemoth::MoveComponent>(cubes[5], BMath::Vector3( 11.0f, 0.0f, -12.0f));
//  	registry.AddComponent<Behemoth::MoveComponent>(cubes[6], BMath::Vector3( 14.0f, -3.0f, -6.0f));
//  	registry.AddComponent<Behemoth::MoveComponent>(cubes[7], BMath::Vector3( 10.0f, 3.0f, -9.0f));


 	playerHandle = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "brick.png", "Player");
 	registry.AddComponent<CameraControllerComponent>(playerHandle, 5.0f, 1.0f, true, Behemoth::KeyCode::KC_Up, Behemoth::KeyCode::KC_Down, Behemoth::KeyCode::KC_Left, Behemoth::KeyCode::KC_Right, Behemoth::KeyCode::KC_Plus, Behemoth::KeyCode::KC_Minus);
 	registry.AddComponent<Behemoth::MoveComponent>(playerHandle, BMath::Vector3(0.0f, 3.0f, 0.0f));
 	registry.AddComponent<Behemoth::RigidBodyComponent>(playerHandle, false);
 	registry.AddComponent<Behemoth::SphereColliderComponent>(playerHandle);
 	registry.AddComponent<Behemoth::ScalingComponent>(playerHandle, BMath::Vector3(1.0));

	auto testCube = gameObjectFactory.AddChildObject(registry, playerHandle,  "SegmentSphere15.obj", "rock.png", "Player");
 	registry.AddComponent<Behemoth::MoveComponent>(testCube, BMath::Vector3(2.0f, 0.0f, 0.0f));
 	registry.AddComponent<Behemoth::StaticComponent>(testCube);
 	registry.AddComponent<Behemoth::OBBColliderComponent>(testCube, BMath::Vector3(1.0f), true, Behemoth::BMask::CollisionType::ChildMesh, Behemoth::BMask::CollisionLayer::NoCollision);
 	registry.AddComponent<Behemoth::ScalingComponent>(testCube, BMath::Vector3(1.0, 1.0, 1.0));



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
		BMath::Vector3 startPos = cameraTransform->worldPosition + cameraTransform->forwardVector * 0.5f;
		BMath::Vector3 endPos = cameraTransform->worldPosition + cameraTransform->forwardVector * distance;

		ECS::EntityHandle debugLineHandle = registry.CreateEntity("Debug line");
		registry.AddComponent<Behemoth::DebugLineComponent>(debugLineHandle, startPos, endPos, 20.0f);

		Behemoth::Ray ray(startPos, cameraTransform->forwardVector);
		std::vector<Behemoth::ContactData> data;
		std::vector<ECS::EntityHandle> entitiesToIgnore;

		Behemoth::RayCast(registry, ray, data, entitiesToIgnore);
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

		if (rot != BMath::Vector3::Zero())
		{
			comp->quat = BMath::Quaternion(BMath::Quaternion(DEGREE_TO_RAD(2.5f), rot));
		}
	}

	if (auto cameraComp = registry.GetComponent<Behemoth::RotationComponent>(mainCameraHandle))
	{
		BMath::Vector3 rot;
		if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_Z))
		{
			rot.y = -1.0f;
		}
		else if (Behemoth::Input::IsKeyHeld(Behemoth::KeyCode::KC_C))
		{
			rot.y = 1.0f;
		}

		if (rot != BMath::Vector3::Zero())
		{
			cameraComp->quat = BMath::Quaternion(BMath::Quaternion(DEGREE_TO_RAD(2.5f), rot));
		}
	}
}

void MainScene::Shutdown()
{

}

void MainScene::InitSystems()
{
	Behemoth::SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}