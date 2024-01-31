#include "pch.h"
#include "TransformSystem.h"
#include "Components/Components.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Misc/TransformHelper.h"

namespace Behemoth
{
	void TransformSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, transformComp] : registry.Get<TransformComponent>())
		{
			if (transformComp->parentIsDirty)
			{
				TransformHelper::UpdateWorldTransform(registry, entity, transformComp);
				TransformHelper::NotifyChildrenTransformChange(registry, entity);
			}
		}
	}
}