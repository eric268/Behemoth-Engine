#include "pch.h"
#include "World.h"
#include "Scene.h"
#include "Core/Log.h"
#include "Systems/SystemHeaders.h"
#include "Renderer/Renderer.h"
#include "Event/Event.h"

namespace Behemoth
{
	World& World::GetInstance()
	{
		static World world = World();
		return world;
	}

	void World::InitializeWorld()
	{
		// Keep these in this order
		Behemoth::SystemManager::GetInstance().AddSystem<CameraSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<TimerSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<MeshInitSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<AudioSystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<ScalingSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<RotationSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<MovementSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<RigidBodySystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<VelocitySystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<BroadCollisionSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<NarrowCollisionSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<CollisionResolutionSystem>();

#ifdef DEBUG
		// These are systems for rendering various debug tools
		Behemoth::SystemManager::GetInstance().AddSystem<WireframeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<DebugLineSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<BoundingVolumeRenderSystem>();
#endif

		Behemoth::SystemManager::GetInstance().AddSystem<SkySphereSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<MeshRenderSystem>();

		// Lighting needs to be run after all primitive render systems have run otherwise will not calculate lighting for systems that run after
		Behemoth::SystemManager::GetInstance().AddSystem<LightingSystem>();
	}

	void World::Update(const float deltaTime)
	{
		if (!currentScene)
		{
			return;
		}

		if (newScene)
		{
			ChangeScene(newScene);
		}

		currentScene->Update(deltaTime);

		// Currently only updating BVH for entities that can move 
		// Will add another method for static entities for when they are created/destroyed
		// currentScene->ReconstructDynamicBVH();
		currentScene->ReconstructBVH<DynamicComponent>();
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

		Renderer::GetInstance().ClearResources();

		currentScene = newScene;

		// This is temporary need to find a way to run system initializations when changing scene, this is for things like 
		// loading any new meshes, moving entities to their world locations etc. For now just running all systems
		Behemoth::SystemManager::GetInstance().Run(0.0f, currentScene->GetRegistry());
		currentScene->CreateScene();
	}

	void World::OnEvent(Event& e)
	{
		if (currentScene)
		{
			currentScene->OnEvent(e);
		}
	}

	void World::Shutdown()
	{
		if (!currentScene)
		{
			return;
		}

		currentScene->Shutdown();
		delete currentScene;
	}
}