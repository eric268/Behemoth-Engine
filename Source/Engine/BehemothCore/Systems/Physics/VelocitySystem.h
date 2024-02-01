#pragma once
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct TransformComponent;

	class VelocitySystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdateLocalTransform(ECS::Registry& registry, const ECS::EntityHandle& entityHandle, TransformComponent* transformComp, const BMath::Vector3& deltaPosition);
	};
}
