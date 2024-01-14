#include "pch.h"
#include "World.h"
#include "Scene.h"
#include "Misc/Log.h"
#include "Systems/SystemManager.h"
#include "Systems/ScalingSystem.h"
#include "Misc/Stopwatch.h"

#include "Systems/Render/MeshRenderSystem.h"
#include "Systems/Render/WireframeRenderSystem.h"
#include "Systems/Render/BoundingVolumeRenderSystem.h"
#include "Systems/Render/DebugLineSystem.h"

#include "Systems/CameraSystem.h"
#include "Systems/RotationSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/MeshInitSystem.h"
#include "Systems/LightingSystem.h"
#include "Systems/VelocitySystem.h"

#include "Systems/Physics/AABBCollisionSystem.h"
#include "Systems/Physics/SphereCollisionSystem.h"
#include "Systems/Physics/OBBCollisionSystem.h"
#include "Systems/Physics/SphereAABBCollisionSystem.h"
#include "Systems/Physics/BroadCollisionSystem.h"
#include "Systems/Physics/NarrowCollisionSystem.h"
#include "Systems/Physics/CollisionResolutionSystem.h"

#include "Systems/Physics/RigidBodySystem.h"

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
		Behemoth::SystemManager::GetInstance().AddSystem<MovementSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<VelocitySystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<MeshInitSystem>();


		Behemoth::SystemManager::GetInstance().AddSystem<CameraSystem>();


		// Behemoth::SystemManager::GetInstance().AddSystem<AABBCollisionSystem>();
		// Behemoth::SystemManager::GetInstance().AddSystem<SphereCollisionSystem>();
		// Behemoth::SystemManager::GetInstance().AddSystem<OBBCollisionSystem>();
		// 	Behemoth::SystemManager::GetInstance().AddSystem<SphereAABBCollisionSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<BroadCollisionSystem>();
 		Behemoth::SystemManager::GetInstance().AddSystem<NarrowCollisionSystem>();
 		Behemoth::SystemManager::GetInstance().AddSystem<CollisionResolutionSystem>();


		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::RigidBodySystem>();

		// These systems should always be last and in this order
		// Maybe make a separate container for them to ensure they are last
		Behemoth::SystemManager::GetInstance().AddSystem<MeshRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<WireframeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<DebugLineSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<BoundingVolumeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<LightingSystem>();
	}

	void World::Update(const float deltaTime)
	{
		if (!currentScene)
		{
			return;
		}

		currentScene->Update(deltaTime);
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