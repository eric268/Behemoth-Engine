#pragma once
#include "ECS/Registry.h"
#include "Physics/BVH.h"

namespace Behemoth
{
	class Event;

	class Scene
	{
	public:
		Scene() = default;

		virtual void OnEvent(Event& e) = 0;
		virtual void Initialize() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Shutdown() = 0;
		virtual ECS::Registry& GetRegistry() = 0;

		void CreateScene();
		void ConstructBVH();
		
		template <typename T>
		void ReconstructBVH()
		{
			if (BVHRootComponent<T>* bvhComp = registry.GetComponent<BVHRootComponent<T>>(staticBVHHandle))
			{
				bvhComp->rootNode = bvhFactory.OnReconstruction<T>(registry, staticBVHEntities);
			}
		}

	protected:
		ECS::Registry registry;

		ECS::EntityHandle staticBVHHandle;
		ECS::EntityHandle dynamicBVHHandle;

		// These are just for debugging purposes for reconstructing a BVH that has drawn the colliders
		std::vector<ECS::EntityHandle> dynamicBVHEntities;
		std::vector<ECS::EntityHandle> staticBVHEntities;

		BVHFactory bvhFactory;
	};
}

