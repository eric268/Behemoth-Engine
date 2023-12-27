#include "pch.h"
#include "MovementSystem.h"
#include "Components/Components.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void MovementSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto movementComponents = registry.GetComponent<MoveComponent>();

		if (!movementComponents->size())
			return;

		auto components = registry.Get<MoveComponent, TransformComponent>();
		for (const auto& [entity, movementComp, transformComp] : components)
		{
			transformComp->dirty = true;
			transformComp->position += movementComp->location;
			transformComp->transformMatrix._41 = movementComp->location.x;
			transformComp->transformMatrix._42 = movementComp->location.y;
			transformComp->transformMatrix._43 = movementComp->location.z;
		}

		for (int i = movementComponents->dense.size() - 1; i >= 0; i--)
		{
			movementComponents->RemoveComponent(movementComponents->dense[i]);
		}
	}
}