#pragma once

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
		
		void ChangeScene(Scene* newScene);
		void Init();
		void Update(const float deltaTime);
		void Shutdown();

	private:
		World() = default;
		Scene* currentScene;
	};
}
