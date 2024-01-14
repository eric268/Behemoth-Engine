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

	class ScalingSystem
	{
	public:
		ScalingSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdateLocalScale(TransformComponent* transformComponent, const BMath::Vector3& oldScale, const BMath::Vector3& newScale);
		void UpdateWorldScale(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComponent, const BMath::Vector3& oldScale, const BMath::Vector3& newScale);
	};
}

