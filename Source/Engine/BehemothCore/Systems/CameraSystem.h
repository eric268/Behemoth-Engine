#pragma once

#include "ECS/System.h"
#include "Components/Components.h"

namespace Behemoth
{
	class CameraSystem : public ECS::System
	{
	public:
		CameraSystem() = default;
		virtual void Run(ECS::Registry& registry) override;

		void UpdatePerspectiveMatrix(CameraComponent& component, const Math::Vector3& position);
	};

}