#pragma once

#include "Math/MathCore.h"
#include "ECS/Entity.h"
// stl
#include <tuple>
#include <vector>

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;

	class CameraHelper
	{
	public:
		static BMath::Matrix4x4f LookAt(const BMath::Vector3& eye, const BMath::Vector3& target, const BMath::Vector3& up);
		static BMath::Vector3 GetMainCameraPostition(ECS::Registry& registry);
		static ECS::Entity GetMainCameraEntity(ECS::Registry& registry);
		static CameraComponent* GetMainCamera(ECS::Registry& registry);
		static TransformComponent* GetMainCameraTransform(ECS::Registry& registry);
};
}

