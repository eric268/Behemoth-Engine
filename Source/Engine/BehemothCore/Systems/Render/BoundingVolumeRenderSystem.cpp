#include "pch.h"
#include "BoundingVolumeRenderSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/CameraHelper.h"

namespace Behemoth
{
	void BoundingVolumeRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<BoundingVolumeComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		BMath::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		// ** Order of multiplication matters here **
		BMath::Matrix4x4 viewProjMatrix = mainCamera->projMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, boundingVolumeComp, transformComp] : components)
		{
			if (!boundingVolumeComp->isVisible)
			{
				continue;
			}

			if (!IsBoundingVolumeInFrustrum( mainCamera, transformComp, boundingVolumeComp))
			{
				continue;
			}

			ProcessBoundingVolume(boundingVolumeComp, transformComp, mainCameraPosition, viewProjMatrix);
		}
	}

	void BoundingVolumeRenderSystem::ProcessBoundingVolume(BoundingVolumeComponent* boundingComp, TransformComponent* transformComp, const BMath::Vector3 cameraPosition, const BMath::Matrix4x4& viewProjMatrix)
	{
		// Scale the sphere mesh by bounding radius
		BMath::Matrix4x4 boundingMatrix = BMath::Matrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			boundingMatrix.data[i][i] = boundingComp->radius * transformComp->worldScale[i];
		}
		boundingMatrix._41 = transformComp->worldTransform._41 + boundingComp->localPosition[0];
		boundingMatrix._42 = transformComp->worldTransform._42 + boundingComp->localPosition[1];
		boundingMatrix._43 = transformComp->worldTransform._43 + boundingComp->localPosition[2];

		ProcessWireframe(boundingComp->mesh, boundingMatrix, viewProjMatrix, true);
	}
}