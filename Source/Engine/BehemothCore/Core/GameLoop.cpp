#include "pch.h"
#include "ECS/Registry.h"
#include "Renderer/Renderer.h"
#include "World/World.h"
#include "World/Scene.h"
#include "Event/EventManager.h"
#include "Systems/SystemManager.h"
#include "Systems/UI/ImageSystem.h"
#include "Systems/UI/TextSystem.h"
#include "Input/Input.h"

extern void CreateApplication();

Behemoth::TextSystem textSystem{};
Behemoth::ImageSystem imageSystem{};

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

void Init()
{
	CreateApplication();
	Behemoth::EventManager::GetInstance().OnEventDelegate = OnEvent;
	Behemoth::EventManager::GetInstance().BindEventCallbacks();
}

void Update(float deltaTime)
{
	deltaTime *= 0.001f;
	ECS::Registry& registry = Behemoth::World::GetInstance().GetActiveScene()->GetRegistry();
	Behemoth::SystemManager::GetInstance().Run(deltaTime, registry);

	Behemoth::World::GetInstance().Update((deltaTime));
	Behemoth::Input::Update((deltaTime));
}

void Render()
{
	Behemoth::Renderer::GetInstance().Draw();

	// TODO:
	// create a separate manager/renderer for handling UI rendering
	Behemoth::Scene* currentScene = Behemoth::World::GetInstance().GetActiveScene();
	if (currentScene)
	{
		ECS::Registry& registry = currentScene->GetRegistry();
		textSystem.Run(0.0f, registry);
		imageSystem.Run(0.0f, registry);
	}
}

void Shutdown()
{
	Behemoth::World::GetInstance().Shutdown();
}
