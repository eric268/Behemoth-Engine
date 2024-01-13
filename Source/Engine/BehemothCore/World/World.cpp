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
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::ScalingSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::RotationSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MovementSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::VelocitySystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MeshInitSystem>();


		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::CameraSystem>();


		// Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::AABBCollisionSystem>();
		// Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::SphereCollisionSystem>();
		// Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::OBBCollisionSystem>();
		// 	Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::SphereAABBCollisionSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::BroadCollisionSystem>();

		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::RigidBodySystem>();

		// These systems should always be last and in this order
		// Maybe make a separate container for them to ensure they are last
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::MeshRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::WireframeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::DebugLineSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::BoundingVolumeRenderSystem>();
		Behemoth::SystemManager::GetInstance().AddSystem<Behemoth::LightingSystem>();
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