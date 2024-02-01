#include "pch.h"
#include "CameraSystem.h"
#include "Components/Components.h"
#include "ECS/Registry.h"
#include "Misc/CameraHelper.h"
#include "NextAPI/App/main.h"
#include "NextAPI/App/AppSettings.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void CameraSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (auto& [entityHandle, cameraComp, transformComp] : registry.Get<CameraComponent, TransformComponent>())
		{
			if (!cameraComp->isInitalized)
			{
				InitializeProjMatrix(cameraComp, transformComp);
			}

			// Only update camera if its transform has changed
			if (cameraComp->isDirty)
			{
				UpdateFrustrum(cameraComp, transformComp);
				SetLook(registry, cameraComp, transformComp);

				// Need separate vectors for camera since usually we will want the world vectors not the local space vectors 
				cameraComp->rightVector =   -BMath::Vector3(cameraComp->viewMatrix._11, cameraComp->viewMatrix._21, cameraComp->viewMatrix._31);
				cameraComp->upVector =       BMath::Vector3(cameraComp->viewMatrix._12, cameraComp->viewMatrix._22, cameraComp->viewMatrix._32);
				cameraComp->forwardVector = -BMath::Vector3(cameraComp->viewMatrix._13, cameraComp->viewMatrix._23, cameraComp->viewMatrix._33);
				cameraComp->isDirty = false;
				transformComp->isDirty = true;
			}
		}
	}

	void CameraSystem::InitializeProjMatrix(CameraComponent* cameraComp, const TransformComponent* transformComp)
	{
		cameraComp->windowWidth = APP_VIRTUAL_WIDTH;
		cameraComp->windowHeight = APP_VIRTUAL_HEIGHT;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComp->FOV) * 0.5f));
		const float aspectRatio = cameraComp->windowWidth / cameraComp->windowHeight;
		const float farPlane = cameraComp->farClippingPlane;
		const float nearPlane = cameraComp->nearClippingPlane;

		cameraComp->projMatrix = BMath::Matrix4x4::Zero();
		cameraComp->projMatrix._11 = fovScale / aspectRatio;
		cameraComp->projMatrix._22 = fovScale;
		cameraComp->projMatrix._33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComp->projMatrix._43 = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComp->projMatrix._34 = -1.0f;

		cameraComp->isInitalized = true;
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComp, TransformComponent* transformComp)
	{
		BMath::Vector3 position = transformComp->worldPosition;
	 	BMath::Vector3 forward = cameraComp->forwardVector;
		BMath::Vector3 up = BMath::Vector3::Up();
 		BMath::Vector3 right = cameraComp->rightVector;

		const float aspectRatio = cameraComp->windowWidth / cameraComp->windowHeight;

		float nearHeight = cameraComp->nearClippingPlane * std::tan(DEGREE_TO_RAD(cameraComp->FOV) * 0.5f);;
		float nearWidth = nearHeight * aspectRatio;

		BMath::Vector3 xAxis, yAxis, zAxis;
		BMath::Vector3 l = position + forward;
		zAxis = (position - l).Normalize();
		xAxis = BMath::Vector3::Cross(up, zAxis);
		yAxis = BMath::Vector3::Cross(zAxis, xAxis);

		BMath::Vector3 nearCenter = position - zAxis * cameraComp->nearClippingPlane;
		BMath::Vector3 farCenter  = position - zAxis * cameraComp->farClippingPlane;

		BMath::Vector3 dirAlongPlane{};
		BMath::Vector3 normal{};

		// Left
		dirAlongPlane = ((nearCenter - xAxis * nearWidth) - position).Normalize();
		BMath::Vector3 leftNormal = BMath::Vector3::Cross(dirAlongPlane, yAxis);
		cameraComp->worldSpaceFrustum[0].normal = leftNormal;
		cameraComp->worldSpaceFrustum[0].d = -BMath::Vector3::Dot(nearCenter - xAxis * nearWidth, leftNormal);

		// Right
		dirAlongPlane = ((nearCenter + xAxis * nearWidth) - position).Normalize();
		BMath::Vector3 rightNormal = BMath::Vector3::Cross(yAxis, dirAlongPlane);
		cameraComp->worldSpaceFrustum[1].normal = rightNormal;
		cameraComp->worldSpaceFrustum[1].d = -BMath::Vector3::Dot(nearCenter + xAxis * nearWidth, rightNormal);

		// Bottom
		dirAlongPlane = ((nearCenter - yAxis * nearHeight) - position).Normalize();
		BMath::Vector3 bottomNormal = BMath::Vector3::Cross(xAxis, dirAlongPlane);
		cameraComp->worldSpaceFrustum[2].normal = bottomNormal;
		cameraComp->worldSpaceFrustum[2].d = -BMath::Vector3::Dot(nearCenter - yAxis * nearHeight, bottomNormal);


		// Top
		dirAlongPlane = ((nearCenter + yAxis * nearHeight) - position).Normalize();
		BMath::Vector3 topNormal = BMath::Vector3::Cross(dirAlongPlane, xAxis);
		cameraComp->worldSpaceFrustum[3].normal = topNormal;
		cameraComp->worldSpaceFrustum[3].d = -BMath::Vector3::Dot(nearCenter + yAxis * nearHeight, topNormal);

		// Near
		cameraComp->worldSpaceFrustum[4].normal = -zAxis;
		cameraComp->worldSpaceFrustum[4].d = -BMath::Vector3::Dot(nearCenter, -zAxis);

		// Far
		cameraComp->worldSpaceFrustum[5].normal = zAxis;
		cameraComp->worldSpaceFrustum[5].d = -BMath::Vector3::Dot(farCenter, zAxis);
	}

	void CameraSystem::SetLook(ECS::Registry& registry, CameraComponent* cameraComp, TransformComponent* cameraTransformComp)
	{
		BMath::Vector3 target = BMath::Vector3::Zero();

		if (cameraComp->focusedEntity.ID == NULL_ENTITY)
		{
			target = cameraTransformComp->worldPosition + cameraTransformComp->forwardVector;
		}
		else
		{
			TransformComponent* transformComp = registry.GetComponent<TransformComponent>(cameraComp->focusedEntity);
			if (transformComp)
			{
				target = transformComp->worldPosition;
			}
		}
		cameraComp->viewMatrix = CameraHelper::LookAt(cameraTransformComp->worldPosition, target, BMath::Vector3::Up());
	}
}