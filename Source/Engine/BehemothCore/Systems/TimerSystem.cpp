#include "pch.h"
#include "TimerSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"

namespace Behemoth
{
	void TimerSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entityHandle, lifetimeComp] : registry.Get<TimerComponent>())
		{
			lifetimeComp->counter += deltaTime;

			if (lifetimeComp->counter >= lifetimeComp->duration)
			{
				lifetimeComp->funcPtr();
				registry.RemoveComponent<TimerComponent>(entityHandle);
			}
		}
	}
}