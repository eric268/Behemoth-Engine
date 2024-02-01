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
	struct ScalingComponent;

	class ScalingSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void ScaleEntities(ECS::Registry& registry, ScalingComponent* scalingComp, TransformComponent* transformComp, const ECS::EntityHandle& entityHandle);
		void UpdateLocalScale(TransformComponent* transformComp, const BMath::Vector3& oldScale, const BMath::Vector3& newScale);
	};
}

