#include "pch.h"

#include "ECS/ECSCore.h"
#include "Renderer/Primitives.h"
#include "Misc/Log.h"
#include "ResourceManager.h"
#include "Renderer/Renderer.h"
#include "World/World.h"
#include "World/Scene.h"
#include "Event/EventManager.h"

#include "ECS/System.h"
#include "Systems/SystemManager.h"
#include "Misc/CameraHelper.h"
#include "Misc/Stopwatch.h"
#include "Systems/ScalingSystem.h"
#include "Systems/Render/MeshRenderSystem.h"
#include "Systems/Render/WireframeRenderSystem.h"
#include "Systems/Render/BoundingVolumeRenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/MeshInitSystem.h"
#include "Systems/LightingSystem.h"
#include "Systems/VelocitySystem.h"

#include "NextAPI/App/app.h"

extern void CreateApplication();



void OnEvent(Behemoth::Event& e)
{
	// Possibly iterate over a container
	Behemoth::World::GetInstance().OnEvent(e);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------

void Init()
{
	CreateApplication();

	Behemoth::EventManager::GetInstance().OnEventDelegate = OnEvent;
	Behemoth::EventManager::GetInstance().BindEventCallbacks();

	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MeshInitSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::RotationSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MovementSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::ScalingSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::CameraSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::VelocitySystem>();

	// These systems should always be last and in this order
	// Maybe make a separate container for them to ensure they are last
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MeshRenderSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::WireframeRenderSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::BoundingVolumeRenderSystem>();
	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::LightingSystem>();
}


//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	Behemoth::World::GetInstance().Update(deltaTime);
	
	// Need to add a check here to ensure that world and active scene are valid
	ECS::Registry& registry = Behemoth::World::GetInstance().GetActiveScene()->GetRegistry();
	Behemoth::SystemManager::GetInstance().Run(deltaTime, registry);
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
