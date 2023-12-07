#include "MovementSystem.h"
#include "Components/Components.h"

namespace Behemoth
{
	void MovementSystem::Run(ECS::Registry& registry)
	{
		auto movementComponents = registry.GetComponent<MovementComponent>();

		if (!movementComponents->size())
			return;

		auto components = registry.Get<MovementComponent, TransformComponent>();
		for (const auto& [movementComp, transformComp] : components)
		{
			transformComp->position += movementComp->location;
			transformComp->transformMatrix[3][0] = movementComp->location.x;
			transformComp->transformMatrix[3][1] = movementComp->location.y;
			transformComp->transformMatrix[3][2] = movementComp->location.z;
		}

		for (int i = movementComponents->dense.size() - 1; i >= 0; i--)
		{
			movementComponents->RemoveComponent(movementComponents->dense[i]);
		}
	}
}