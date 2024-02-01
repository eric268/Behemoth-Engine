#pragma once

#include "Math/MathCore.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct TransformComponent;
	struct MoveComponent;

	// This system essentially is a translate function that will teleport entities to the inputted location initially ignoring all collisions
	// Should preferably use the Velocity Component to move dynamic objects in a scene
	class MovementSystem
	{
	public:
		MovementSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdateLocalTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, TransformComponent* transformComp, BMath::Vector3 deltaLocation);
	};
}

