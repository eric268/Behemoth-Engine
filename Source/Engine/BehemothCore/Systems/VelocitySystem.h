#pragma once
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class TransformComponent;

	class VelocitySystem
	{
	public:
		VelocitySystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdateLocalTransform(TransformComponent* transformComp, const BMath::Vector3& deltaPosition);
		void UpdateWorldTransform(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComp, const BMath::Vector3& deltaPosition);
	};

}
