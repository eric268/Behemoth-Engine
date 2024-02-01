#pragma once

namespace Behemoth
{
	class Scene;
	class Registry;
	struct Event;

	class World
	{
	public:
		World(World&) = delete;
		World& operator=(const World&) = delete;

		static World& GetInstance();

		void InitializeWorld();
		void Update(const float deltaTime);
		Scene* GetActiveScene();
		void ChangeScene(Scene* newScene);
		void OnEvent(Event& e);
		void Shutdown();

	private:
		World() = default;

		Scene* currentScene;
		Scene* newScene;
	};
}
