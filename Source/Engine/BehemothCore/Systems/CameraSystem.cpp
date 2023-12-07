#include "CameraSystem.h"
#include "Misc/CameraHelper.h"
#include "main.h"

namespace Behemoth
{
	void CameraSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<CameraComponent, TransformComponent>();

		for (auto& [cameraComp, transformComp] : components)
		{
			UpdatePerspectiveMatrix(*cameraComp, transformComp->position);
		}
	}

	void CameraSystem::UpdatePerspectiveMatrix(CameraComponent& component, const Math::Vector3& position)
	{
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

		component.viewMatrix = CameraHelper::LookAt(position, Math::Vector3(0, 0, 1), Math::Vector3(0, 1, 0));
	}
}