#include "pch.h"
#include "VelocitySystem.h"
#include "Components/Components.h"

namespace Behemoth
{
	void VelocitySystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<VelocityComponent, TransformComponent>();
		for (const auto& [entity, velocityComp, transformComp] : components)
		{
			if (Math::Vector3::Equals(velocityComp->velocity, Math::Vector3::Zero()))
				continue;

			Math::Vector3 diff = velocityComp->velocity * deltaTime;

			transformComp->dirty = true;
			transformComp->position += diff;
			transformComp->transformMatrix._41 = diff.x;
			transformComp->transformMatrix._42 = diff.y;
			transformComp->transformMatrix._43 = diff.z;
		}
	}
}
