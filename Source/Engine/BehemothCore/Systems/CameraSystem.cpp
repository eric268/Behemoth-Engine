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
			if (cameraComp->isDirty)
			{
				UpdatePerspectiveMatrix(cameraComp, transformComp);
				UpdateFrustrum(cameraComp);
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

		cameraComponent->projMatrix = BMath::Matrix4x4::Zero();
		cameraComponent->projMatrix._11 = fovScale / aspectRatio;
		cameraComponent->projMatrix._22 = fovScale;
		cameraComponent->projMatrix._33 = -(farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._43 = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->projMatrix._34 = -1.0f;
		cameraComponent->projMatrix._44 = 1.0f;

		BMath::Vector3 target = transformComponent->worldPosition + transformComponent->forwardVector;

		cameraComponent->viewMatrix = CameraHelper::LookAt(transformComponent->worldPosition, target, BMath::Vector3(0,1,0));
		cameraComponent->inverseTransposeViewMatrix = BMath::Matrix4x4::Transpose(BMath::Matrix4x4::Inverse(cameraComponent->viewMatrix));
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent)
	{
		auto m =  cameraComponent->projMatrix * cameraComponent->viewMatrix;

		cameraComponent->worldSpaceFrustum[0].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(0)).Normalize();
		cameraComponent->worldSpaceFrustum[0].distance = (m.data[3][3] + m.data[0][3]);

		cameraComponent->worldSpaceFrustum[1].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(0)).Normalize();
		cameraComponent->worldSpaceFrustum[1].distance = (m.data[3][3] - m.data[0][3]);

		cameraComponent->worldSpaceFrustum[2].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(1)).Normalize();
		cameraComponent->worldSpaceFrustum[2].distance = (m.data[3][3] + m.data[1][3]);

		cameraComponent->worldSpaceFrustum[3].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(1)).Normalize();
		cameraComponent->worldSpaceFrustum[3].distance = (m.data[3][3] - m.data[1][3]);

		cameraComponent->worldSpaceFrustum[4].normal = BMath::Vector3(m.GetRow(3) + m.GetRow(2)).Normalize();
		cameraComponent->worldSpaceFrustum[4].distance = (m.data[3][3] + m.data[2][3]);

		cameraComponent->worldSpaceFrustum[5].normal = BMath::Vector3(m.GetRow(3) - m.GetRow(2)).Normalize();
		cameraComponent->worldSpaceFrustum[5].distance = (m.data[3][3] - m.data[2][3]) + 100.0f;

	}
	float CameraSystem::GetDistance(const BMath::Matrix4x4& m, int index)
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