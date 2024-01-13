#include "pch.h"
#include "Scene.h"
#include "Components/PhysicsComponents.h"


namespace Behemoth
{
	void Scene::CreateScene()
	{
		Initalize();
		ConstructBVH();
	}

	void Scene::ConstructBVH()
	{
		BVHFactory factory{};

		staticBVHHandle = registry.CreateEntity("Static BVH");
		std::shared_ptr<BVHNode> staticBVHNode = factory.OnConstruction<StaticComponent>(registry, staticBVHEntities);
		registry.AddComponent<BVHComponent<StaticComponent>>(staticBVHHandle, staticBVHNode);

// 		dynamicBVHHandle = registry.CreateEntity("Dynamic BVH");
// 		std::shared_ptr<BVHNode> dynamicVHNode = factory.OnConstruction<RigidBodyComponent>(registry, dynamicBVHEntities);
// 		registry.AddComponent<BVHComponent<RigidBodyComponent>>(dynamicBVHHandle, dynamicVHNode);
	}
}