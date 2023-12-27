#pragma once

#include "ECS/System.h"
#include "Components/Components.h"

namespace Behemoth
{
	class CameraSystem
	{
	public:
		CameraSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdatePerspectiveMatrix(CameraComponent* component, const Math::Vector3& position);
		void UpdateFrustrum(CameraComponent* cameraComponent);
	};

}