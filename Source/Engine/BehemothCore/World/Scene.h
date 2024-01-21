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
		virtual void Initalize() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Shutdown() = 0;
		virtual ECS::Registry& GetRegistry() = 0;

		void CreateScene();
		void ConstructBVH();
		void RecalculateBVH();

	protected:
		ECS::Registry registry;

		std::vector<ECS::EntityHandle> staticBVHEntities;
		std::vector<ECS::EntityHandle> dynamicBVHEntities;
		ECS::EntityHandle staticBVHHandle;
		ECS::EntityHandle dynamicBVHHandle;

		BVHFactory factory;
	};
}

