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
		BMath::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, boundingVolumeComp, transformComp] : components)
		{
			if (!boundingVolumeComp->isVisible)
				continue;

			if (!IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolumeComp->volumeRadius))
			{
				continue;
			}

			ProcessBoundingVolume(boundingVolumeComp, mainCameraPosition, transformComp->worldTransform, viewProjMatrix);
		}
	}

	void BoundingVolumeRenderSystem::ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, const BMath::Vector3 cameraPosition, const BMath::Matrix4x4& transformMatrix, const BMath::Matrix4x4& viewProjMatrix)
	{
		// Scale the sphere mesh by bounding radius
		BMath::Matrix4x4 boundingMatrix = BMath::Matrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			boundingMatrix.data[i][i] = boundingVolumeComponent->volumeRadius;
		}
		boundingMatrix = transformMatrix * boundingMatrix;

		ProcessWireframe(boundingVolumeComponent->mesh, boundingMatrix, viewProjMatrix, true);
	}
}