#include "pch.h"
#include "World.h"
#include "Scene.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void World::ChangeScene(Scene* newScene)
	{
		if (!newScene)
		{
			LOG_ERROR("Attempted to change to null scene");
			return;
		}

		if (currentScene)
		{
			currentScene->Shutdown();
			delete currentScene;
		}

		currentScene = newScene;
		currentScene->Init();
	}

	Scene* World::GetActiveScene()
	{
		if (!currentScene)
		{
			LOG_ERROR("Current Scene is null");
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
	}

	void World::OnEvent(Event& e)
	{
		if (currentScene)
		{
			currentScene->OnEvent(e);
		}
	}
}