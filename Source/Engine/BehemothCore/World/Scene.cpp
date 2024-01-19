#include "pch.h"
#include "Scene.h"
#include "Components/PhysicsComponents.h"
#include "Core/Log.h"
#include "Event/EventManager.h"
#include "Event/WindowEvents.h"
#include "Misc/CameraHelper.h"


namespace Behemoth
{
	void Scene::OnEvent(Event& e)
	{
		Behemoth::EventHandler eventHandler{e};

		bool handled = eventHandler.ProcessEvent<Behemoth::WindowResizeEvent>([&](Behemoth::WindowResizeEvent keyEvent)
			{
				const ECS::EntityHandle cameraHandle = Behemoth::CameraHelper::GetMainCameraEntity(registry);

				if (cameraHandle.ID != NULL_ENTITY)
				{
					Behemoth::CameraComponent* cameraComponent = registry.GetComponent<Behemoth::CameraComponent>(cameraHandle);
					if (cameraComponent)
					{
						cameraComponent->isDirty = true;
					}
				}
			});

		// If event is not handled by parent scene then propagate event to current active scene
		if (!handled)
		{
			ProcessEvent(e);
		}
	}
	void Scene::CreateScene()
	{
		Initalize();
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
		auto staticBVHComp = registry.GetComponent<BVHRootComponent<StaticComponent>>(staticBVHHandle);
		if (staticBVHComp)
		{
			staticBVHComp->rootNode = factory.OnReconstruction<StaticComponent>(registry, staticBVHEntities);
		}

		auto dynamicBVHComp = registry.GetComponent<BVHRootComponent<RigidBodyComponent>>(dynamicBVHHandle);
		if (dynamicBVHComp)
		{
			dynamicBVHComp->rootNode = factory.OnReconstruction<RigidBodyComponent>(registry, dynamicBVHEntities);
		}
	}
}