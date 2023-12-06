
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

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

BehemothEngine::Primitives* p;
ECS::Registry registry;
ECS::RenderSystem renderSystem;
ECS::ModelInitalizeSystem loadingSystem;
ECS::CameraSystem cameraSystem;
ECS::RotationSystem rotationSystem;
ECS::MovementSystem movementSystem;

void Init()
{
	std::cout << std::filesystem::current_path();

	ECS::Entity e0 = registry.CreateEntity("Main Camera");
	registry.AddComponent<ECS::CameraComponent>(e0, true);
	registry.AddComponent<ECS::TransformComponent>(e0);
	registry.AddComponent<ECS::MovementComponent>(e0, Math::Vector3(0, 0, 10));


	ECS::Entity e1 = registry.CreateEntity("Cube 1");
	registry.AddComponent<ECS::MeshComponent>(e1, "cube.obj", "brick.png");
	registry.AddComponent<ECS::TransformComponent>(e1);
	registry.AddComponent<ECS::MeshInitalizeComponent>(e1);
	registry.AddComponent<ECS::RotationComponent>(e1, 1, 2.0f);
	registry.AddComponent<ECS::MovementComponent>(e1, Math::Vector3(0, 0, 0));

	ECS::Entity e2 = registry.CreateEntity("Debug Sphere");
	registry.AddComponent<ECS::MeshComponent>(e2, "sphere.obj", "rock.png");
	registry.AddComponent<ECS::TransformComponent>(e2);
	registry.AddComponent<ECS::MeshInitalizeComponent>(e2);
	registry.AddComponent<ECS::RotationComponent>(e2, 1, 2.0f);
	registry.AddComponent<ECS::MovementComponent>(e2, Math::Vector3(-3, 0, 0));

// 
// 	ECS::Entity e3 = registry.CreateEntity("Debug Icosphere");
// 	registry.AddComponent<ECS::MeshComponent>(e3, "Models/icosphere.obj");
// 	registry.AddComponent<ECS::TransformComponent>(e3);
// 	registry.AddComponent<ECS::MeshInitalizeComponent>(e3);
// 	registry.AddComponent<ECS::RotationComponent>(e3, 2, 2.0f);
// 	registry.AddComponent<ECS::MovementComponent>(e3, Math::Vector3(3, 0, 0));
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	loadingSystem.Run(registry);
	movementSystem.Run(registry);
	cameraSystem.Run(registry);
	rotationSystem.Run(registry);

#ifdef DEBUG
	std::cout << "Debugging\n";
#endif // DEBUG

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
