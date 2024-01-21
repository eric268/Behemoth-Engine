#include "pch.h"
#include "TestScene.h"
#include "MainScene.h"

#include "Components/Components.h"
#include "Components/LightComponents.h"
#include "Components/PhysicsComponents.h"
#include "Components/RenderComponents.h"
#include "Components/AudioComponents.h"

#include "Physics/Collision/BroadCollision.h"
#include "Physics/Collision/CollisionMask.h"
#include "Physics/Ray.h"

#include "Components/UIComponents.h"

#include "Factories/SkySphereFactory.h"

TestScene::TestScene()
{
	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 0));


	LOGMESSAGE(General, "Main Scene constructed\n");
}

void TestScene::Initalize()
{
	// Function called after scene constructor 
	// Can be used for additional initialization steps that are required post construction
}

void TestScene::ProcessEvent(Behemoth::Event& e)
{
	// Processes general engine events such as window close, resize etc.
	// Does not process window events, use static Input library to check mouse/keyboard/controller events
}

void TestScene::Update(const float deltaTime)
{
	if (Behemoth::RotationComponent* parentRotationComponent = registry.GetComponent<Behemoth::RotationComponent>(exampleParentEntity))
	{
		parentRotationComponent->quat = BMath::Quaternion(DEGREE_TO_RAD(1.5f), BMath::Vector3(0, 0, 1));
	}

	if (Behemoth::Input::IsKeyDown(Behemoth::KeyCode::KC_Space))
	{
		Behemoth::Scene* mainScene = new MainScene();
		Behemoth::World::GetInstance().ChangeScene(mainScene);
	}
}

void TestScene::Shutdown()
{

}