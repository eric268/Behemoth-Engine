#include "pch.h"
#include "Scene.h"
#include "Components/PhysicsComponents.h"
#include "Core/Log.h"
#include "Event/EventManager.h"
#include "Event/WindowEvents.h"
#include "Misc/CameraHelper.h"


namespace Behemoth
{
	void Scene::CreateScene()
	{
		Initialize();
		ConstructBVH();
	}

	void Scene::ConstructBVH()
	{
		staticBVHHandle = registry.CreateEntity("Static BVH");
		std::shared_ptr<BVHNode> staticBVHNode = factory.OnConstruction<StaticComponent>(registry, staticBVHEntities);
		registry.AddComponent<BVHRootComponent<StaticComponent>>(staticBVHHandle, staticBVHNode);
  
		dynamicBVHHandle = registry.CreateEntity("Dynamic BVH");
		std::shared_ptr<BVHNode> dynamicVHNode = factory.OnConstruction<RigidBodyComponent>(registry, dynamicBVHEntities);
		registry.AddComponent<BVHRootComponent<RigidBodyComponent>>(dynamicBVHHandle, dynamicVHNode);
	}

	void Scene::RecalculateBVH()
	{
// 		auto staticBVHComp = registry.GetComponent<BVHRootComponent<StaticComponent>>(staticBVHHandle);
// 		if (staticBVHComp)
// 		{
// 			staticBVHComp->rootNode = factory.OnReconstruction<StaticComponent>(registry, staticBVHEntities);
// 		}

		auto dynamicBVHComp = registry.GetComponent<BVHRootComponent<RigidBodyComponent>>(dynamicBVHHandle);
		if (dynamicBVHComp)
		{
			dynamicBVHComp->rootNode = factory.OnReconstruction<RigidBodyComponent>(registry, dynamicBVHEntities);
		}
	}
}