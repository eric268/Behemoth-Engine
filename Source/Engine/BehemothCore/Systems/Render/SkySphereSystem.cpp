#include "pch.h"
#include "SkySphereSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"

namespace Behemoth
{
	void SkySphereSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* cameraComp = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransform = CameraHelper::GetMainCameraTransform(registry);

		for (const auto& [entity, transformComp, skySphere] : registry.Get<TransformComponent, SkySphereComponent>())
		{
			transformComp->worldPosition = cameraTransform->worldPosition;
			transformComp->worldTransform._41 = transformComp->worldPosition.x;
			transformComp->worldTransform._42 = transformComp->worldPosition.y;
			transformComp->worldTransform._43 = transformComp->worldPosition.z;
			transformComp->isDirty = true;
		}
	}
}