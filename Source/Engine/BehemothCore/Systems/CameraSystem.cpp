#include "pch.h"
#include "CameraSystem.h"
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
				UpdatePerspectiveMatrix(cameraComp, transformComp->position);
				UpdateFrustrum(cameraComp);
				cameraComp->isDirty = false;
			}
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent* cameraComponent, const Math::Vector3& position)
	{

		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		cameraComponent->windowWidth = rect.right - rect.left;
		cameraComponent->windowHeight = rect.bottom - rect.top;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f));
		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		const float farPlane = cameraComponent->farClippingPlane;
		const float nearPlane = cameraComponent->nearClippingPlane;

		cameraComponent->perspectiveMatrix = Math::Matrix4x4::Zero();
		cameraComponent->perspectiveMatrix._11 = fovScale / aspectRatio;
		cameraComponent->perspectiveMatrix._22 = fovScale;
		cameraComponent->perspectiveMatrix._33 = (-farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->perspectiveMatrix._43 = (-2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->perspectiveMatrix._34 = -1.0f;

		cameraComponent->viewMatrix = CameraHelper::LookAt(position, Math::Vector3(0.0f, 0.0f, -5.0f), Math::Vector3(0, 1, 0));
		cameraComponent->inverseTransposeViewMatrix = Math::Matrix4x4::Inverse(cameraComponent->viewMatrix);
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent)
	{
		float thetaY = DEGREE_TO_RAD(cameraComponent->FOV);
		float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		float thetaX = 2.0f * std::atan(std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f) * aspectRatio);

		thetaY *= 0.5f;
		thetaX *= 0.5f;

		float dist = 0.0f;

		Math::Vector3 nearNormal = Math::Vector3(0.0f, 0.0f, -1.0f);
		cameraComponent->worldSpaceFrustum[0] = Math::Plane::TransformPlane(Math::Plane(nearNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);

		Math::Vector3 farNormal = Math::Vector3(0.0f, 0.0f, 1.0f);
		cameraComponent->worldSpaceFrustum[1] = Math::Plane::TransformPlane(Math::Plane(farNormal, -cameraComponent->farClippingPlane), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around y axis
		Math::Vector3 leftNormal = Math::Vector3(-std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		cameraComponent->worldSpaceFrustum[2] = Math::Plane::TransformPlane(Math::Plane(leftNormal, dist), cameraComponent->inverseTransposeViewMatrix);
		
		// Rotate around y axis
		Math::Vector3 rightNormal = Math::Vector3(std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		cameraComponent->worldSpaceFrustum[3] = Math::Plane::TransformPlane(Math::Plane(rightNormal, dist), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around x axis
		Math::Vector3 bottomNormal = Math::Vector3(0.0f, -std::cos(thetaY), -std::sin(thetaY)).Normalize();
		cameraComponent->worldSpaceFrustum[4] = Math::Plane::TransformPlane(Math::Plane(bottomNormal, dist), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around x axis
		Math::Vector3 topNormal = Math::Vector3(0.0f, std::cos(thetaY), -std::sin(thetaY)).Normalize();
		cameraComponent->worldSpaceFrustum[5] = Math::Plane::TransformPlane(Math::Plane(topNormal, dist), cameraComponent->inverseTransposeViewMatrix);
	}
}