#include "pch.h"
#include "CameraSystem.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "NextAPI/App/main.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void CameraSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent>();

		for (auto& [entity, cameraComp, transformComp] : components)
		{
			// Only update camera if its transform has changed
			if (cameraComp->isDirty)
			{
				UpdatePerspectiveMatrix(cameraComp, transformComp);
				UpdateFrustrum(cameraComp, transformComp);
				cameraComp->isDirty = false;
			}
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent* cameraComponent, const TransformComponent* transformComponent)
	{
		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		cameraComponent->windowWidth = rect.right - rect.left;
		cameraComponent->windowHeight = rect.bottom - rect.top;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f));
		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		const float farPlane = cameraComponent->farClippingPlane;
		const float nearPlane = cameraComponent->nearClippingPlane;

		cameraComponent->projMatrix = BMath::BMatrix4x4::Zero();
		cameraComponent->projMatrix._11 = fovScale / aspectRatio;
		cameraComponent->projMatrix._22 = fovScale;
		cameraComponent->projMatrix._33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._43 = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._34 = -1.0f;

		BMath::Vector3 target = transformComponent->worldPosition + transformComponent->forwardVector;

		cameraComponent->viewMatrix = CameraHelper::LookAt(transformComponent->worldPosition, target, BMath::Vector3::Up());
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent, TransformComponent* transformComp)
	{
		BMath::Vector3 position = transformComp->worldPosition;
	 	BMath::Vector3 forward = transformComp->forwardVector;
		BMath::Vector3 up = BMath::Vector3::Up();
 		BMath::Vector3 right = transformComp->rightVector;

		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;

		float nearHeight = cameraComponent->nearClippingPlane * std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f);;
		float nearWidth = nearHeight * aspectRatio;

		BMath::Vector3 xAxis, yAxis, zAxis;
		BMath::Vector3 l = position + forward;
		zAxis = (position - l).Normalize();
		xAxis = BMath::Vector3::Cross(up, zAxis);
		yAxis = BMath::Vector3::Cross(zAxis, xAxis);

		BMath::Vector3 nearCenter = position - zAxis * cameraComponent->nearClippingPlane;
		BMath::Vector3 farCenter  = position - zAxis * cameraComponent->farClippingPlane;

		BMath::Vector3 dirAlongPlane{};
		BMath::Vector3 normal{};

		// Left
		dirAlongPlane = ((nearCenter - xAxis * nearWidth) - position).Normalize();
		BMath::Vector3 leftNormal = BMath::Vector3::Cross(dirAlongPlane, yAxis);
		cameraComponent->worldSpaceFrustum[0].normal = leftNormal;
		cameraComponent->worldSpaceFrustum[0].d = -BMath::Vector3::Dot(nearCenter - xAxis * nearWidth, leftNormal);

		// Right
		dirAlongPlane = ((nearCenter + xAxis * nearWidth) - position).Normalize();
		BMath::Vector3 rightNormal = BMath::Vector3::Cross(yAxis, dirAlongPlane);
		cameraComponent->worldSpaceFrustum[1].normal = rightNormal;
		cameraComponent->worldSpaceFrustum[1].d = -BMath::Vector3::Dot(nearCenter + xAxis * nearWidth, rightNormal);

		// Bottom
		dirAlongPlane = ((nearCenter - yAxis * nearHeight) - position).Normalize();
		BMath::Vector3 bottomNormal = BMath::Vector3::Cross(xAxis, dirAlongPlane);
		cameraComponent->worldSpaceFrustum[2].normal = bottomNormal;
		cameraComponent->worldSpaceFrustum[2].d = -BMath::Vector3::Dot(nearCenter - yAxis * nearHeight, bottomNormal);


		// Top
		dirAlongPlane = ((nearCenter + yAxis * nearHeight) - position).Normalize();
		BMath::Vector3 topNormal = BMath::Vector3::Cross(dirAlongPlane, xAxis);
		cameraComponent->worldSpaceFrustum[3].normal = topNormal;
		cameraComponent->worldSpaceFrustum[3].d = -BMath::Vector3::Dot(nearCenter + yAxis * nearHeight, topNormal);

		// Near
		cameraComponent->worldSpaceFrustum[4].normal = -zAxis;
		cameraComponent->worldSpaceFrustum[4].d = -BMath::Vector3::Dot(nearCenter, -zAxis);

		// Far
		cameraComponent->worldSpaceFrustum[5].normal = zAxis;
		cameraComponent->worldSpaceFrustum[5].d = -BMath::Vector3::Dot(farCenter, zAxis);
	}

	float CameraSystem::GetDistance(const BMath::BMatrix4x4& m, int index)
	{
		if (index > 3)
		{
			index = 0;
		}

		static BMath::Vector3 normals[4] = 
		{
			BMath::Vector3(1,0,0), 
			BMath::Vector3(-1,0,0), 
			BMath::Vector3(0,1,0), 
			BMath::Vector3(0,-1,0)
		};

		BMath::Vector4 transformedNormal = m * BMath::Vector4(normals[index], 1.0f);
		BMath::Vector3 result = BMath::Vector3(transformedNormal) / transformedNormal.w;
		return BMath::Vector3::Magnitude(result);
	}
}