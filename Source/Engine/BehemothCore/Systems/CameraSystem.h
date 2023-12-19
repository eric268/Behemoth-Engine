#pragma once

#include "ECS/System.h"
#include "Components/Components.h"

namespace Behemoth
{
	class CameraSystem
	{
	public:
		CameraSystem() = default;
		void Run(ECS::Registry& registry);

	private:
		void UpdatePerspectiveMatrix(CameraComponent& component, const Math::Vector3& position);
		void UpdateFrustrum(const CameraComponent& cameraComponent, FrustrumComponent& frustrumComponent);
	};

}