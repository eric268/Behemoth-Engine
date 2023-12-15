#include "CameraSystem.h"
#include "Misc/CameraHelper.h"
#include "main.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void CameraSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent, FrustrumComponent>();

		for (auto& [entity, cameraComp, transformComp, frustrumComp] : components)
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
		component.perspectiveMatrix._11 = fovScale / aspectRatio;
		component.perspectiveMatrix._22 = fovScale;
		component.perspectiveMatrix._33 = (-farPlane + nearPlane) / (farPlane - nearPlane);
		component.perspectiveMatrix._43 = (-2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
		component.perspectiveMatrix._34 = -1.0f;

		component.viewMatrix = CameraHelper::LookAt(position, Math::Vector3(0.0f, 0.0f, -1.0f), Math::Vector3(0, 1, 0));
		component.inverseTransposeViewMatrix = Math::Matrix4x4::Inverse(component.viewMatrix);
		component.isDirty = false;
	}

	void CameraSystem::UpdateFrustrum(const CameraComponent& cameraComponent, FrustrumComponent& frustrumComponent)
	{
		float thetaY = DEGREE_TO_RAD(cameraComponent.FOV);
		float aspectRatio = static_cast<float>(cameraComponent.windowWidth) / static_cast<float>(cameraComponent.windowHeight);
		float thetaX = 2.0f * std::atan(std::tan(DEGREE_TO_RAD(cameraComponent.FOV) * 0.5f) * aspectRatio);

		thetaY *= 0.5f;
		thetaX *= 0.5f;

		float dist = 0.0f;

		Math::Vector3 nearNormal = Math::Vector3(0.0f, 0.0f, -1.0f);
		frustrumComponent.worldSpacePlanes[0] = Math::Plane::TransformPlane(Math::Plane(nearNormal, cameraComponent.nearClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 farNormal = Math::Vector3(0.0f, 0.0f, 1.0f);
		frustrumComponent.worldSpacePlanes[1] = Math::Plane::TransformPlane(Math::Plane(farNormal, -cameraComponent.farClippingPlane), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 leftNormal = Math::Vector3(-std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		frustrumComponent.worldSpacePlanes[2] = Math::Plane::TransformPlane(Math::Plane(leftNormal, dist), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 rightNormal = Math::Vector3(std::sin(thetaX), 0.0f, -std::cos(thetaX)).Normalize();
		frustrumComponent.worldSpacePlanes[3] = Math::Plane::TransformPlane(Math::Plane(rightNormal, dist), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 bottomNormal = Math::Vector3(0.0f, -std::cos(thetaY), -std::sin(thetaY)).Normalize();
		frustrumComponent.worldSpacePlanes[4] = Math::Plane::TransformPlane(Math::Plane(bottomNormal, dist), cameraComponent.inverseTransposeViewMatrix);

		Math::Vector3 topNormal = Math::Vector3(0.0f, std::cos(thetaY), -std::sin(thetaY)).Normalize();
		frustrumComponent.worldSpacePlanes[5] = Math::Plane::TransformPlane(Math::Plane(topNormal, dist), cameraComponent.inverseTransposeViewMatrix);
	}
}