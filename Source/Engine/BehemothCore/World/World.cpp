#include "pch.h"
#include "World.h"
#include "Scene.h"
#include "Misc/Log.h"
#include "Systems/SystemHeaders.h"

namespace Behemoth
{
	void World::ChangeScene(Scene* newScene)
	{
		if (!newScene)
		{
			LOGMESSAGE(MessageType::Error, "Attempted to change to null scene");
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
		currentScene->CreateScene();
	}

	Scene* World::GetActiveScene()
	{
		if (!currentScene)
		{
			LOGMESSAGE(MessageType::Error, "Current Scene is null");
			return nullptr;
		}

		return currentScene;
	}

	void World::Init()
	{
		// Keep these in this order
		Behemoth::SystemManager::GetInstance().AddSystem<ScalingSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<RotationSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<VelocitySystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<CameraSystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<MeshInitSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<TimerSystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::RigidBodySystem>();

 		Behemoth::SystemManager::GetInstance().AddSystem<BroadCollisionSystem>();
  		Behemoth::SystemManager::GetInstance().AddSystem<NarrowCollisionSystem>();
  		Behemoth::SystemManager::GetInstance().AddSystem<CollisionResolutionSystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<MovementSystem>();

		// These systems should always be last and in this order
		// Maybe make a separate container for them to ensure they are last
		Behemoth::SystemManager::GetInstance().AddSystem<AudioSystem>();


#ifdef DEBUG
		// These are systems for rendering various debug tools
		Behemoth::SystemManager::GetInstance().AddSystem<WireframeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<DebugLineSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<BoundingVolumeRenderSystem>();
#endif

		Behemoth::SystemManager::GetInstance().AddSystem<SkySphereSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<MeshRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<LightingSystem>();
	}

	void World::Update(const float deltaTime)
	{
		if (!currentScene)
		{
			return;
		}

		currentScene->Update(deltaTime);
		currentScene->RecalculateBVH();
	}

	void World::Shutdown()
	{
		if (!currentScene)
			return;

		currentScene->Shutdown();
		delete currentScene;

		// Clear all resource managers
	}

	void World::OnEvent(Event& e)
	{
		if (currentScene)
		{
			currentScene->OnEvent(e);
		}
	}
}