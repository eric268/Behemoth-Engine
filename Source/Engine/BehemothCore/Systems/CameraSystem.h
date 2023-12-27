#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;

	class CameraSystem
	{
	public:
		CameraSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdatePerspectiveMatrix(CameraComponent* component, const TransformComponent* transformComponent);
		void UpdateFrustrum(CameraComponent* cameraComponent);
	};

}