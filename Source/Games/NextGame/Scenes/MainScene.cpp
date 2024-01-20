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
	environmentLighting = registry.CreateEntity("Environment Lighting");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(environmentLighting);
	registry.AddComponent<Behemoth::AmbientLightComponent>(environmentLighting);
	skySphere = Behemoth::SkySphereFactory::CreateSkySphere(registry, "SeamlessSky.png", { 1.0, 1.0 });

	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<MoveComponent>(mainCameraHandle, BMath::Vector3(0, 0, 10));

	cameraSpringArm = registry.CreateEntity("Spring arm");
	registry.AddComponent<TransformComponent>(cameraSpringArm);
	registry.AddComponent<RotationComponent>(cameraSpringArm);

	GameObjectFactory gameObjectFactory{};

	

	groundEntity = gameObjectFactory.CreateGameObject(registry, "plane.obj", "brick.png", "Ground entity", { 8,8 });
	registry.AddComponent<MoveComponent>(groundEntity, BMath::Vector3(0, -1, 0));
	registry.AddComponent<ScalingComponent>(groundEntity, BMath::Vector3(10, 1, 10));
	registry.AddComponent<OBBColliderComponent>(groundEntity, BMath::Vector3(1.0f));
	registry.AddComponent<StaticComponent>(groundEntity);
	// registry.AddComponent<WireframeComponent>(groundEntity, "plane.obj", BMath::Vector3(1, 0.1, 1));



	playerEntity = gameObjectFactory.CreateGameObject(registry, "", "", "Player");
	registry.AddComponent<RigidBodyComponent>(playerEntity, false);
	registry.AddComponent<MoveComponent>(playerEntity, BMath::Vector3(0, 10, 10));
	registry.AddComponent<OBBColliderComponent>(playerEntity);
	// registry.AddComponent<WireframeComponent>(playerEntity, "cube.obj", BMath::Vector3(1));

	projectileEntity = registry.CreateEntity("Projectile");
	registry.AddComponent<TransformComponent>(projectileEntity);

 	projectileMeshHandle = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "rock.png", "Projectile Mesh");
 	registry.AddComponent<RigidBodyComponent>(playerEntity, false);


	arrowIconEntity = gameObjectFactory.CreateGameObject(registry, "arrow.obj", "arrow.jpg", "Arrow icon");
	registry.AddComponent<ScalingComponent>(arrowIconEntity, BMath::Vector3(0.25));
	registry.AddComponent<MoveComponent>(arrowIconEntity, BMath::Vector3(0, 0, -3));

	BMath::Quaternion q1 = BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(0, 0, 1));
	BMath::Quaternion q2 = BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(1, 0, 0));
	registry.AddComponent<RotationComponent>(arrowIconEntity, q1 * q2, true);

	// gameObjectFactory.AddChildObject(registry, projectileEntity, mainCameraHandle);

	CameraComponent* mainCameraComp = registry.GetComponent<CameraComponent>(mainCameraHandle);
	mainCameraComp->focusedEntity = playerEntity;


	gameObjectFactory.AddChildObject(registry, playerEntity, cameraSpringArm);
	gameObjectFactory.AddChildObject(registry, cameraSpringArm, mainCameraHandle);

	gameObjectFactory.AddChildObject(registry, playerEntity, projectileEntity);
	gameObjectFactory.AddChildObject(registry, projectileEntity, arrowIconEntity);
	gameObjectFactory.AddChildObject(registry, projectileEntity, projectileMeshHandle);
	
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
	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(projectileMeshHandle))
	{
		if (VelocityComponent* velocity = registry.GetComponent<VelocityComponent>(playerEntity))
		{
			float speed = velocity->velocity.Magnitude();
			BMath::Vector3 dir = BMath::Vector3::Normalize(BMath::Vector3(-velocity->velocity.z, 0.0f, -velocity->velocity.x));
			parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(5.0f), dir);
			parentRotationComponent->isAdditive = true;

		}
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(projectileEntity))
	{
		auto input = Input::GetRightControllerAnaloge(0);
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* t = registry.GetComponent<TransformComponent>(projectileEntity);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), t->upVector);
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), t->rightVector);
		}

		parentRotationComponent->quat = quatX * quatY;
		parentRotationComponent->isAdditive = true;
	}

	if (Behemoth::RotationComponent* parentRotationComponent = registry.AddComponent<Behemoth::RotationComponent>(cameraSpringArm))
	{
		auto input = Input::GetLeftControllerAnaloge(0);
		BMath::Quaternion quatX = BMath::Quaternion::Identity();
		BMath::Quaternion quatY = BMath::Quaternion::Identity();

		TransformComponent* t = registry.GetComponent<TransformComponent>(cameraSpringArm);

		if (input.x != 0.0f)
		{
			quatX = BMath::Quaternion(DEGREE_TO_RAD(input.x), BMath::Vector3(t->upVector));
		}

		if (input.y != 0.0f)
		{
			quatY = BMath::Quaternion(DEGREE_TO_RAD(-input.y), BMath::Vector3(t->rightVector));
		}

		parentRotationComponent->quat = quatY * quatX;
		parentRotationComponent->isAdditive = true;
	}

	//  
	if (Input::IsKeyHeld(KeyCode::KC_Space))
	{

		TransformComponent* playerTransform = registry.GetComponent<TransformComponent>(projectileEntity);


		BMath::Vector3 vel = ProjectileMotion::CalculateInitalVelocity(5.0f, playerTransform->forwardVector);
	
		auto playerVelocity = registry.AddComponent<VelocityComponent>(playerEntity);
		playerVelocity->velocity = vel;

		auto playerRigidBody = registry.AddComponent<RigidBodyComponent>(playerEntity);
		playerRigidBody->affectedByGravity = true;

		// counter += deltaTime;
// 		if (std::abs(counter) > 2)
// 		{
// 			// counter = (counter > 0 ? 1 : -1); // Limit counter to range [-2, 2]
// 		}
// 
// 		TransformComponent* parentTransform = registry.GetComponent<TransformComponent>(projectileEntity);
// 		TransformComponent* childTransform = registry.GetComponent<TransformComponent>(arrowIconEntity);
// 
// 		BMath::Vector3 newScale = BMath::Vector3(0.25f) + BMath::Vector3(counter * 3); // initialScaleOfChild should be defined elsewhere
// 		childTransform->localScale = newScale;
// 
// 		float movementFactor = 0.05f;
// 		BMath::Vector3 movement = parentTransform->forwardVector * movementFactor * counter;
// 
// 		std::cout << newScale.Print() << std::endl;
// 
// 		registry.AddComponent<ScalingComponent>(arrowIconEntity, BMath::Vector3(0.5));
//  		registry.AddComponent<MoveComponent>(arrowIconEntity,  movement);
	}

	if (Input::IsKeyDown(KeyCode::KC_B))
	{
		VelocityComponent* velocityComponent = registry.GetComponent<VelocityComponent>(playerEntity);
		velocityComponent->velocity = BMath::Vector3(0.0f);
	}
}

void MainScene::Shutdown()
{

}