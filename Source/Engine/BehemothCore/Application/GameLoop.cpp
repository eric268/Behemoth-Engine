
#include "ECS/ECSCore.h"
#include "Render/Primitives.h"
#include "Misc/Log.h"
#include "ResourceManager.h"
#include "Render/Renderer.h"

#include "ECS/System.h"
#include "Misc/CameraHelper.h"
#include "Misc/Stopwatch.h"
#include "Systems/ScalingSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/MeshInitSystem.h"
#include "Systems/LightingSystem.h"

#include <windows.h> 
//stl
#include <iostream>
#include <filesystem>

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
Behemoth::LightingSystem lightingSystem;

void Init()
{
	ECS::Entity e0 = registry.CreateEntity("Main Camera");
	registry.AddComponent<Behemoth::CameraComponent>(e0, true);
	registry.AddComponent<Behemoth::FrustrumComponent>(e0);
	registry.AddComponent<Behemoth::TransformComponent>(e0);
	registry.AddComponent<Behemoth::MovementComponent>(e0, Math::Vector3(0.0f, 0.0f, 5.0f));

	ECS::Entity e1 = registry.CreateEntity("Cube 1");
	registry.AddComponent<Behemoth::MeshComponent>(e1, "monkey.obj", "diamond.png");
	registry.AddComponent<Behemoth::TransformComponent>(e1);
	registry.AddComponent<Behemoth::MeshInitalizeComponent>(e1);
	registry.AddComponent<Behemoth::RotationComponent>(e1, 1, 1.0f);
	registry.AddComponent<Behemoth::MovementComponent>(e1, Math::Vector3(0.f, 0.0f, -5.0f));
	registry.AddComponent<Behemoth::ScalingComponent>(e1, Math::Vector3(1.0f, 1.0f, 1.0f));
	registry.AddComponent<Behemoth::BoundingVolumeComponent>(e1, 1.5f, true);

	ECS::Entity e2 = registry.CreateEntity("Directional Light");
	registry.AddComponent<Behemoth::DirectionalLightComponent>(e2, Math::Vector3(-0.707f, 0, -0.707f),
		Math::Vector3(0.25f, 0.25f, 0.25f) ,3.0f);

	ECS::Entity e3 = registry.CreateEntity("Point Light 1");
	registry.AddComponent<Behemoth::PointLightComponent>(e3,Math::Vector3(0.75f, 0.75f, 0.75f), 2.5f);
	registry.AddComponent<Behemoth::TransformComponent>(e3);
	registry.AddComponent<Behemoth::MovementComponent>(e3, Math::Vector3(6.0f, 0.0f, 4.0f));

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


	// These systems should always be last and in this order
	// Maybe make a separate container for them to ensure they are last
	renderSystem.Run(registry);
	lightingSystem.Run(registry);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	Behemoth::Renderer::GetInstance().Draw();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
