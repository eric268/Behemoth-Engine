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

#include "GameSystems/PCSystem.h"

#include "TestScene.h"

#include "Scripts/GameObjects/PlayerFactory.h"

using namespace Behemoth;

MainScene::MainScene()
{
	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });



	playerCharacter = PlayerFactory::CreatePlayer(registry, BMath::Vector3(0, 10, 10));

	GameObjectFactory gameObjectFactory{};
	obstacleHandle = gameObjectFactory.CreateGameObject(registry, "cube.obj", "brick.png");
	registry.AddComponent<ScalingComponent>(obstacleHandle, BMath::Vector3(3, 3, 0.1));
	registry.AddComponent<MoveComponent>(obstacleHandle, BMath::Vector3(0, 10, 0));
	registry.AddComponent<OBBColliderComponent>(obstacleHandle);
	registry.AddComponent<StaticComponent>(obstacleHandle);

	

	groundEntity = gameObjectFactory.CreateGameObject(registry, "plane.obj", "brick.png", "Ground entity", { 8,8 });
	registry.AddComponent<MoveComponent>(groundEntity, BMath::Vector3(0, 0, 0));
	registry.AddComponent<ScalingComponent>(groundEntity, BMath::Vector3(10, 1.1f, 10));
	registry.AddComponent<OBBColliderComponent>(groundEntity, BMath::Vector3(1.0f));
	registry.AddComponent<StaticComponent>(groundEntity);
	registry.AddComponent<RotationComponent>(groundEntity, BMath::Quaternion(DEGREE_TO_RAD(1), BMath::Vector3(1,0,0)));
	// registry.AddComponent<WireframeComponent>(groundEntity, "plane.obj", BMath::Vector3(1, 0.1, 1));

}

void MainScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
	InitalizeSystems();
}

void MainScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void MainScene::Update(const float deltaTime)
{
// 	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerCharacter.playerMeshHandle))
// 	{
// 		if (VelocityComponent* velocity = registry.GetComponent<VelocityComponent>(playerCharacter.playerHandle))
// 		{
// 			float speed = velocity->velocity.Magnitude();
// 			BMath::Vector3 dir = BMath::Vector3::Normalize(BMath::Vector3(-velocity->velocity.z, 0.0f, -velocity->velocity.x));
// 			parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(5.0f), dir);
// 			parentRotationComponent->isAdditive = true;
// 
// 		}
// 	}
// 
// 	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(playerCharacter.projectileHandle))
// 	{
// 		auto input = Input::GetRightControllerAnaloge(0);
// 		BMath::Quaternion quatX = BMath::Quaternion::Identity();
// 		BMath::Quaternion quatY = BMath::Quaternion::Identity();
// 
// 		TransformComponent* projectileTransform = registry.GetComponent<TransformComponent>(playerCharacter.projectileHandle);
// 
// 		if (input.x != 0.0f)
// 		{
// 			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), projectileTransform->upVector);
// 		}
// 
// 		if (input.y != 0.0f)
// 		{
// 			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), projectileTransform->rightVector);
// 		}
// 
// 		parentRotationComponent->quat = (quatX * quatY).Normalize();
// 
// 		// parentRotationComponent->quat = quatX * quatY;
// 		parentRotationComponent->isAdditive = true;
// 	}

// 	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(cameraSpringArm))
// 	{
// 		auto input = Input::GetLeftControllerAnaloge(0);
// 		BMath::Quaternion quatX = BMath::Quaternion::Identity();
// 		BMath::Quaternion quatY = BMath::Quaternion::Identity();
// 
// 		TransformComponent* t = registry.GetComponent<TransformComponent>(cameraSpringArm);
// 
// 		if (input.x != 0.0f)
// 		{
// 			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), BMath::Vector3(t->upVector));
// 		}
// 
// 		if (input.y != 0.0f)
// 		{
// 			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(t->rightVector));
// 		}
// 
// 		parentRotationComponent->quat = quatY * quatX;
// 		parentRotationComponent->isAdditive = true;
// 	}

// 	//  
// 	if (Input::IsKeyReleased(KeyCode::KC_Space) || Input::GetRightControllerTrigger(0) == 0.0f)
// 	{
// 		if (counter > 0.0f)
// 		{
// 			MeshComponent* arrowMesh = registry.GetComponent<MeshComponent>(playerCharacter.arrowMeshHandle);
// 			if (arrowMesh)
// 			{
// 				arrowMesh->isVisible = false;
// 			}
// 
// 			TransformComponent* playerTransform = registry.GetComponent<TransformComponent>(playerCharacter.projectileHandle);
// 
// 			BMath::Vector3 vel = ProjectileMotion::CalculateInitalVelocity(counter, playerTransform->forwardVector);
// 
// 			auto playerVelocity = registry.AddComponent<VelocityComponent>(playerCharacter.playerHandle);
// 			playerVelocity->velocity = vel;
// 
// 			auto playerRigidBody = registry.AddComponent<RigidBodyComponent>(playerCharacter.playerHandle);
// 			playerRigidBody->affectedByGravity = true;
// 
// 			counter = 0.0f;
// 		}
// 	}
// 	if (Input::IsKeyHeld(KeyCode::KC_Space) || Input::GetRightControllerTrigger(0) > 0.0f)
// 	{
// 		float powerCharge = counter + deltaTime * powerChargeSpeed * Input::GetRightControllerTrigger(0);
// 
// 		counter = std::min(100.0f, powerCharge);
// 		std::cout << counter << std::endl;
// 	}

// 	if (Input::IsKeyDown(KeyCode::KC_B))
// 	{
// 		VelocityComponent* velocityComponent = registry.GetComponent<VelocityComponent>(playerCharacter.playerHandle);
// 		velocityComponent->velocity = BMath::Vector3(0.0f);
// 
// 		MeshComponent* arrowMesh = registry.GetComponent<MeshComponent>(playerCharacter.arrowMeshHandle);
// 		if (arrowMesh)
// 		{
// 			arrowMesh->isVisible = true;
// 		}
// 	}
}

void MainScene::InitalizeSystems()
{
	SystemManager::GetInstance().AddSystem<PCSystem>();
	SystemManager::GetInstance().AddSystem<CameraControllerSystem>();
}

void MainScene::Shutdown()
{

}