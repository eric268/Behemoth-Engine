#pragma once

#include "Systems/Render/RenderSystem.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct SkySphereComponent;
	struct CameraComponent;
	struct TransformComponent;

	class SkySphereSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);
	};

}

