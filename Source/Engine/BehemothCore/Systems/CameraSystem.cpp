#include "CameraSystem.h"
#include "Misc/CameraHelper.h"
#include "main.h"

namespace Behemoth
{
	void CameraSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent, FrustrumComponent>();

		for (auto& [cameraComp, transformComp, frustrumComp] : components)
		{
			UpdatePerspectiveMatrix(*cameraComp, transformComp->position);
			UpdateFrustrum(*cameraComp, *frustrumComp);
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent& component, const Math::Vector3& position)
	{
// 		if (!component.isDirty)
// 			return;

		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		component.windowWidth = rect.right - rect.left;
		component.windowHeight = rect.bottom - rect.top;

		const float fovScale = 1.0f / (std::tan(DEGREE_TO_RAD(component.FOV) * 0.5f));
		const float aspectRatio = component.windowWidth / component.windowHeight;
		const float farPlane = component.farClippingPlane;
		const float nearPlane = component.nearClippingPlane;

		component.perspectiveMatrix = Math::Matrix4x4::Zero();
		component.perspectiveMatrix[0][0] = fovScale / aspectRatio;
		component.perspectiveMatrix[1][1] = fovScale;
		component.perspectiveMatrix[2][2] = -(farPlane - nearPlane) / (farPlane - nearPlane);
		component.perspectiveMatrix[3][2] = (-2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		component.perspectiveMatrix[2][3] = -1.0f;

		component.viewMatrix = CameraHelper::LookAt(position, Math::Vector3(0, 0, -1), Math::Vector3(0, 1, 0));
		component.inverseTransposeViewMatrix = Math::Matrix4x4::Transpose(Math::Matrix4x4::Inverse(component.viewMatrix));
		component.isDirty = false;
	}

	void CameraSystem::UpdateFrustrum(const CameraComponent& cameraComponent, FrustrumComponent& frustrumComponent)
	{
		float tanHalfFOVY = tan(DEGREE_TO_RAD(cameraComponent.FOV) * 0.5f);
		float tanHalfFOVX = tanHalfFOVY * (cameraComponent.windowHeight / cameraComponent.windowWidth);

		Math::Vector3 nearNormal = Math::Vector3(0.0f, 0.0f, -1.0f);
		frustrumComponent.worldSpacePlanes[0] = Math::Plane::TransformPlane(Math::Plane(nearNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 farNormal = Math::Vector3(0.0f, 0.0f, 1.0f);
		frustrumComponent.worldSpacePlanes[1] = Math::Plane::TransformPlane(Math::Plane(farNormal, cameraComponent.farClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 leftNormal = Math::Vector3(tanHalfFOVX, 0.0f, -1.0f).Normalize();
		frustrumComponent.worldSpacePlanes[2] = Math::Plane::TransformPlane(Math::Plane(leftNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 rightNormal = Math::Vector3(-tanHalfFOVX, 0.0f, -1.0f).Normalize();
		frustrumComponent.worldSpacePlanes[3] = Math::Plane::TransformPlane(Math::Plane(rightNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 bottomNormal = Math::Vector3(0.0f, tanHalfFOVY, -1.0f).Normalize();
		frustrumComponent.worldSpacePlanes[4] = Math::Plane::TransformPlane(Math::Plane(bottomNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);
		
		Math::Vector3 topNormal = Math::Vector3(0.0f, -tanHalfFOVY, -1.0f).Normalize();
		frustrumComponent.worldSpacePlanes[5] = Math::Plane::TransformPlane(Math::Plane(topNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);
	}
}