#pragma once
#include "Event/Event.h"

#include <queue>

namespace Behemoth
{
	class Scene;
	class Registry;

	class World
	{
	public:
		World(World&) = delete;
		World& operator=(const World&) = delete;

		static World& GetInstance()
		{
			static World world = World();
			return world;
		}

		Scene* GetActiveScene();
		void OnEvent(Event& e);
		void ChangeScene(Scene* newScene);
		void Init();
		void Update(const float deltaTime);
		void Shutdown();
		void ProcessCollisions(const float deltaTime);

	private:
		World() = default;

		Scene* currentScene;
	};
}
