#pragma once
#include "ECS/Registry.h"

namespace Behemoth
{
	class Scene
	{
	public:
		Scene();

		virtual void Init() = 0;
		virtual void Update(const float deltaTime);
		virtual void Shutdown();

		ECS::Registry registry;
	private:

	};
}

