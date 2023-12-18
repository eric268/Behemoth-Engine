
#include "ECS/ECSCore.h"
#include "Render/Primitives.h"
#include "Misc/Log.h"
#include "ResourceManager.h"
#include "Render/Renderer.h"
#include "World/World.h"
#include "World/Scene.h"

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

Behemoth::RenderSystem renderSystem;
Behemoth::MeshInitSystem loadingSystem;
Behemoth::CameraSystem cameraSystem;
Behemoth::RotationSystem rotationSystem;
Behemoth::MovementSystem movementSystem;
Behemoth::ScalingSystem scalingSystem;
Behemoth::LightingSystem lightingSystem;

extern void CreateApplication();

void Init()
{
	CreateApplication();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	Behemoth::World::GetInstance().Update(deltaTime);

	ECS::Registry& registry = Behemoth::World::GetInstance().GetActiveScene()->registry;

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
	Behemoth::World::GetInstance().Shutdown();
}
