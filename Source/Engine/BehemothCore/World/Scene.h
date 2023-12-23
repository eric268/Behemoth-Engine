#pragma once
#include "ECS/Registry.h"

namespace Behemoth
{
	class Event;

	class Scene
	{
	public:
		Scene() = default;

		virtual void OnEvent(Event& e) = 0;
		virtual void Init() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void Shutdown() = 0;
		virtual ECS::Registry& GetRegistry() = 0;

	protected:
		ECS::Registry registry;
	};
}

