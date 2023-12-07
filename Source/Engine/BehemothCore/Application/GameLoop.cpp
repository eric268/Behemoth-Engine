
#include <windows.h> 
#include <iostream>
#include "ECS/ECSCore.h"
#include "Render/Primitives.h"
#include "Misc/Log.h"
#include "ResourceManager.h"

#include "ECS/System.h"
#include "Misc/CameraHelper.h"
//stl
#include <iostream>
#include <filesystem>

#include "Systems/ScalingSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/MeshInitSystem.h"

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

ECS::Registry registry;
Behemoth::RenderSystem renderSystem;
Behemoth::MeshInitSystem loadingSystem;
Behemoth::CameraSystem cameraSystem;
Behemoth::RotationSystem rotationSystem;
Behemoth::MovementSystem movementSystem;
Behemoth::ScalingSystem scalingSystem;

void Init()
{
	ECS::Entity e0 = registry.CreateEntity("Main Camera");
	registry.AddComponent<Behemoth::CameraComponent>(e0, true);
	registry.AddComponent<Behemoth::TransformComponent>(e0);
	registry.AddComponent<Behemoth::MovementComponent>(e0, Math::Vector3(0, 0, 0));

	ECS::Entity e1 = registry.CreateEntity("Cube 1");
	registry.AddComponent<Behemoth::MeshComponent>(e1, "cube.obj", "rock.png");
	registry.AddComponent<Behemoth::TransformComponent>(e1);
	registry.AddComponent<Behemoth::MeshInitalizeComponent>(e1);
	registry.AddComponent<Behemoth::RotationComponent>(e1, 0, 1.0f);
	registry.AddComponent<Behemoth::MovementComponent>(e1, Math::Vector3(0, 0, 5));
	

	registry.AddComponent<Behemoth::ScalingComponent>(e1, Math::Vector3(1.0f, 1.0f, 1.0f));

	ECS::Entity e2 = registry.CreateEntity("Sphere 1");
	registry.AddComponent<Behemoth::MeshComponent>(e2, "sphere.obj", "brick.png");
	registry.AddComponent<Behemoth::TransformComponent>(e2);
	registry.AddComponent<Behemoth::MeshInitalizeComponent>(e2);
	registry.AddComponent<Behemoth::RotationComponent>(e2, 1, 2.5f);
	registry.AddComponent<Behemoth::MovementComponent>(e2, Math::Vector3(3, 0, 5));
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	loadingSystem.Run(registry);
	movementSystem.Run(registry);
	rotationSystem.Run(registry);
	cameraSystem.Run(registry);
	scalingSystem.Run(registry);

}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	renderSystem.Run(registry);
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
