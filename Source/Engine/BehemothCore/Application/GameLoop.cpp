
#include <windows.h> 
#include <iostream>
#include "ECS/ECSCore.h"
#include "Render/Primitives.h"
#include "Misc/Log.h"
#include "ResourceManager.h"

#include "ECS/System.h"
//stl
#include <iostream>

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

BehemothEngine::Primitives* p;
ECS::Registry registry;
ECS::RenderSystem renderSystem;
ECS::ModelLoadingSystem loadingSystem;
ECS::CameraSystem cameraSystem;

void Init()
{
	ECS::Entity e1 = registry.CreateEntity("Entity 1");

	registry.AddComponent<ECS::MeshComponent>(e1, "Models/cube.obj");

	loadingSystem.Run(registry);

	//BehemothEngine::ResourceManager::GetInstance().LoadMesh("Models/cube.obj");
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	cameraSystem.Run(registry);
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
