#pragma once

#include "Math/MathCore.h"

namespace ECS
{
	class Registry;
	struct EntityHandle;
}

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;

	class CameraHelper
	{
	public:
		static BMath::BMatrix4x4 LookAt(const BMath::Vector3& eye, const BMath::Vector3& target, const BMath::Vector3& up);
		static BMath::Vector3 GetMainCameraPostition(ECS::Registry& registry);
		static const ECS::EntityHandle& GetMainCameraEntity(ECS::Registry& registry);
		static CameraComponent* GetMainCamera(ECS::Registry& registry);
		static TransformComponent* GetMainCameraTransform(ECS::Registry& registry);
	};
}

