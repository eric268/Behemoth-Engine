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

			Math::Vector3 delta = velocityComp->velocity / deltaTime;

			transformComp->position += delta;
			transformComp->transformMatrix._41 += delta.x;
			transformComp->transformMatrix._42 += delta.y;
			transformComp->transformMatrix._43 += delta.z;
			transformComp->dirty = true;

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent && cameraComponent->isMain)
			{
				cameraComponent->isDirty = true;
			}
		}
	}
}
