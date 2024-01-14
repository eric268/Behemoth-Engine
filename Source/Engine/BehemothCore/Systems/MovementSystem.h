#pragma once

#include "Math/MathCore.h"
#include "ECS/Entity.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct TransformComponent;
	struct MoveComponent;

	class MovementSystem
	{
	public:
		MovementSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdateLocalTransform(TransformComponent* transformComp, MoveComponent* movementComp);
		void UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, MoveComponent* movementComp);
	};
}

