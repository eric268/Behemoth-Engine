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
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		// ** Order of multiplication matters here **
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, boundingVolumeComp, transformComp] : components)
		{
			if (!boundingVolumeComp->isVisible)
				continue;

			if (!IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolumeComp->volumeRadius))
			{
				continue;
			}

			ProcessBoundingVolume(boundingVolumeComp, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix);
		}
	}

	void BoundingVolumeRenderSystem::ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix)
	{
		// Scale the sphere mesh by bounding radius
		Math::Matrix4x4 boundingMatrix = Math::Matrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			boundingMatrix.data[i][i] = boundingVolumeComponent->volumeRadius;
		}
		boundingMatrix = transformMatrix * boundingMatrix;

		ProcessWireframe(boundingVolumeComponent->mesh, boundingMatrix, viewProjMatrix, Math::Vector3::One(), true);
	}
}