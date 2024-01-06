#include "pch.h"
#include "World.h"
#include "Scene.h"
#include "Misc/Log.h"
#include "Systems/SystemManager.h"

namespace Behemoth
{
	void World::ChangeScene(Scene* newScene)
	{
		if (!newScene)
		{
			LOG_MESSAGE(MessageType::Error, "Attempted to change to null scene");
			return;
		}

		if (currentScene)
		{
			currentScene->Shutdown();
			delete currentScene;
		}

		currentScene = newScene;

		// This is temporary need to find a way to run system initializations when changing scene, this is for things like loading any new meshes, moving entities to their world locations etc
		// For now just running all systems
		Behemoth::SystemManager::GetInstance().Run(0.0f, currentScene->GetRegistry());
		currentScene->Init();
	}

	Scene* World::GetActiveScene()
	{
		if (!currentScene)
		{
			LOG_MESSAGE(MessageType::Error, "Current Scene is null");
			return nullptr;
		}

		return currentScene;
	}

	void World::Init()
	{
		if (!currentScene)
			return;

		currentScene->Init();
	}

	void World::Update(const float deltaTime)
	{
		if (!currentScene)
			return;

		currentScene->Update(deltaTime);
	}

	void World::Shutdown()
	{
		if (!currentScene)
			return;

		currentScene->Shutdown();
		delete currentScene;
	}

	void World::OnEvent(Event& e)
	{
		if (currentScene)
		{
			currentScene->OnEvent(e);
		}
	}
}