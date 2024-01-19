#include "pch.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Components/AudioComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "GameSystems/CameraControllerSystem.h"
#include "GameComponents/CameraControllerComponent.h"
#include "Components/UIComponents.h"

#include "Factories/SkySphereFactory.h"

#include "Physics/ProjectileMotion.h"

#include "TestScene.h"

using namespace Behemoth;

MainScene::MainScene()
{
	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 25));

	secondCameraHandle = cameraFactory.CreateCamera(registry, false, "Second Camera");
	registry.AddComponent<RotationComponent>(secondCameraHandle, BMath::Quaternion(DEGREE_TO_RAD(20), BMath::Vector3(1, 0, 0)));


	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);

	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "brick.png", { 8.0, 8.0 });

  	GameObjectFactory gameObjectFactory{};
	exampleParentEntity = gameObjectFactory.CreateGameObject(registry, "monkey.obj", "rock.png", "Example Parent");
	registry.AddComponent<MoveComponent>(exampleParentEntity, BMath::Vector3(0.0f, 0.0f, 0.0f));
	registry.AddComponent<OBBColliderComponent>(exampleParentEntity);
	registry.AddComponent<RigidBodyComponent>(exampleParentEntity, false);
	registry.AddComponent<ScalingComponent>(exampleParentEntity, BMath::Vector3(1.5f));

	exampleChildEntity1 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "cube.obj", "brick.png", "Child 1");
	registry.AddComponent<MoveComponent>(exampleChildEntity1, BMath::Vector3(-3.0f, 0.0f, 0.0f));
	registry.AddComponent<RigidBodyComponent>(exampleChildEntity1, false);

	exampleChildEntity2 = gameObjectFactory.AddChildObject(registry, exampleParentEntity, "sphere.obj", "brick.png", "Child 2");
	registry.AddComponent<MoveComponent>(exampleChildEntity2, BMath::Vector3(3.0f, 0.0f, 0.0f));
	registry.AddComponent<RigidBodyComponent>(exampleChildEntity2, false);

	Behemoth::LightFactory lightFactory{};
	pointLight = lightFactory.CreatePointLight(registry);
	registry.AddComponent<Behemoth::MoveComponent>(pointLight, BMath::Vector3(0.0f,0.0f,-2.0f));

 }

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
	SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}

void MainScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void MainScene::Update(const float deltaTime)
{
	registry.AddComponent<Behemoth::RotationComponent>(exampleParentEntity, BMath::Quaternion(DEGREE_TO_RAD(5.0f), BMath::Vector3(0, 1, 0)), true);

	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(mainCameraHandle))
	{
		float rot = 0.0f;
		if (Input::IsKeyHeld(KeyCode::KC_Z))
		{
			rot -= 2.0f;
		}
		if (Input::IsKeyHeld(KeyCode::KC_C))
		{
			rot += 2.0f;
		}
		parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(rot), BMath::Vector3(0, 1, 0));
	}
//  
 	if (Input::IsKeyDown(KeyCode::KC_Space))
 	{
 		GameObjectFactory gameObjectFactory{};
 
 		auto mainCameraTransform = registry.GetComponent<TransformComponent>(mainCameraHandle);
 		BMath::Vector3 vel = ProjectileMotion::CalculateInitalVelocity(15.0f, 0, DEGREE_TO_RAD(45.0f));
 		vel.z *= -1.0f;
 
 		projectileObject = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "brick.png", "Parent Projectile");
 		registry.AddComponent<VelocityComponent>(projectileObject, vel);
 		registry.AddComponent<RigidBodyComponent>(projectileObject, true);

 		// BMath::Vector3 pos = mainCameraTransform->worldPosition + mainCameraTransform->forwardVector * 1.5f;
 
 		BMath::Vector3 pos = BMath::Vector3(0,0,25);
 
 		registry.AddComponent<MoveComponent>(projectileObject, pos);
 		registry.AddComponent<ScalingComponent>(projectileObject, BMath::Vector3(0.5f));

 		auto parent =registry.AddComponent<ParentComponent>(projectileObject);

// 		auto child = gameObjectFactory.AddChildObject(registry, projectileObject, "sphere.obj", "brick.obj", "Child Projectile");
// 		registry.AddComponent<MoveComponent>(child, BMath::Vector3(0, 0, 1));
 
		auto mainCameraComponent = registry.GetComponent<CameraComponent>(mainCameraHandle);
		mainCameraComponent->isMain = false;
		auto secondCameraComponent = registry.GetComponent<CameraComponent>(secondCameraHandle);
		secondCameraComponent->isMain = true;
 
  		registry.AddComponent<ChildComponent>(secondCameraHandle, projectileObject);
  		parent->childHandles.push_back(secondCameraHandle);
  
  		registry.AddComponent<MoveComponent>(secondCameraHandle, BMath::Vector3(0, 3, 6));
	}

	if (Input::IsKeyDown(KeyCode::KC_B))
	{
		registry.RemoveComponent<ChildComponent>(secondCameraHandle);
		registry.AddComponent<MoveComponent>(secondCameraHandle, BMath::Vector3(0, 0, 25));
		// registry.DestroyEntity(projectileObject);
	}
}

void MainScene::Shutdown()
{

}