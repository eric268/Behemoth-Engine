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

		// std::cout << "Width: " << cameraComponent->windowWidth << ' ' << " Height: " << cameraComponent->windowHeight << std::endl;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f));
		const float aspectRatio = cameraComponent->windowWidth / cameraComponent->windowHeight;
		const float farPlane = cameraComponent->farClippingPlane;
		const float nearPlane = cameraComponent->nearClippingPlane;

		cameraComponent->perspectiveMatrix = BMath::Matrix4x4::Zero();
		cameraComponent->perspectiveMatrix._11 = fovScale / aspectRatio;
		cameraComponent->perspectiveMatrix._22 = fovScale;
		cameraComponent->perspectiveMatrix._33 = (-farPlane + nearPlane) / (farPlane - nearPlane);
		cameraComponent->perspectiveMatrix._43 = (-2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		cameraComponent->perspectiveMatrix._34 = -1.0f;

		BMath::Vector3 target = transformComponent->position + transformComponent->forwardVector;


		cameraComponent->viewMatrix = CameraHelper::LookAt(transformComponent->position, target, BMath::Vector3::Up());
		cameraComponent->inverseTransposeViewMatrix = BMath::Matrix4x4::Inverse(cameraComponent->viewMatrix);
	}

	void CameraSystem::UpdateFrustrum(CameraComponent* cameraComponent)
	{
		float thetaY = DEGREE_TO_RAD(cameraComponent->FOV);
		float aspectRatio = cameraComponent->windowHeight / cameraComponent->windowWidth;
		float thetaX = 2.0f * std::atan(std::tan(DEGREE_TO_RAD(cameraComponent->FOV) * 0.5f) * aspectRatio);

		thetaY *= 0.5f;
		thetaX *= 0.5f;

		float dist = 0.0f;

		BMath::Vector3 nearNormal = BMath::Vector3(0.0f, 0.0f, -1.0f);
		cameraComponent->worldSpaceFrustum[0] = Plane::TransformPlane(Plane(nearNormal, cameraComponent->nearClippingPlane), cameraComponent->inverseTransposeViewMatrix);

		BMath::Vector3 farNormal = BMath::Vector3(0.0f, 0.0f, 1.0f);
		cameraComponent->worldSpaceFrustum[1] = Plane::TransformPlane(Plane(farNormal, -cameraComponent->farClippingPlane), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around y axis
		BMath::Vector3 leftNormal = BMath::Vector3(-std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		cameraComponent->worldSpaceFrustum[2] = Plane::TransformPlane(Plane(leftNormal, dist), cameraComponent->inverseTransposeViewMatrix);
		
		// Rotate around y axis
		BMath::Vector3 rightNormal = BMath::Vector3(std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		cameraComponent->worldSpaceFrustum[3] = Plane::TransformPlane(Plane(rightNormal, dist), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around x axis
		BMath::Vector3 bottomNormal = BMath::Vector3(0.0f, -std::cos(thetaY), -std::sin(thetaY)).Normalize();
		cameraComponent->worldSpaceFrustum[4] = Plane::TransformPlane(Plane(bottomNormal, dist), cameraComponent->inverseTransposeViewMatrix);

		// Rotate around x axis
		BMath::Vector3 topNormal = BMath::Vector3(0.0f, std::cos(thetaY), -std::sin(thetaY)).Normalize();
		cameraComponent->worldSpaceFrustum[5] = Plane::TransformPlane(Plane(topNormal, dist), cameraComponent->inverseTransposeViewMatrix);
	}
}