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

#include "Input/Input.h"

#include "NextAPI/App/app.h"

extern void CreateApplication();



void OnEvent(Behemoth::Event& e)
{
	// If event is consumed by input then do not propagate to world/scene level
	if (e.GetEventFlags() & Behemoth::Events::EventFlags::Input)
	{
		Behemoth::Input::OnEvent(e);
		return;
	}

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
}


//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	deltaTime *= 0.001f;

	ECS::Registry& registry = Behemoth::World::GetInstance().GetActiveScene()->GetRegistry();
	Behemoth::SystemManager::GetInstance().Run(deltaTime, registry);

	Behemoth::World::GetInstance().Update((deltaTime));
	Behemoth::Input::Update((deltaTime));
	// Need to add a check here to ensure that world and active scene are valid
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
