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
		// Storing handles for drawing / deleting debugging colliders
		staticBVHHandle = registry.CreateEntity("Static BVH");
		std::shared_ptr<BVHNode> staticBVHNode = bvhFactory.OnConstruction<StaticComponent>(registry, staticBVHEntities);
		registry.AddComponent<BVHRootComponent<StaticComponent>>(staticBVHHandle, staticBVHNode);
  
		// Using VelocityComponent as a filter between static and dynamic entities, assumed that if it has a velocity component
		// then the object can move and therefore the BVH tree needs to be updated more frequently
 		dynamicBVHHandle = registry.CreateEntity("Dynamic BVH");
 		std::shared_ptr<BVHNode> dynamicBVHNode = bvhFactory.OnConstruction<DynamicComponent>(registry, dynamicBVHEntities);
 		registry.AddComponent<BVHRootComponent<DynamicComponent>>(dynamicBVHHandle, dynamicBVHNode);
	}
}