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
	class CameraComponent;
	class TransformComponent;

	class CameraHelper
	{
	public:
		static Math::Matrix4x4 LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up);
		static Math::Vector3 GetMainCameraPostition(ECS::Registry& registry);
		static ECS::Entity GetMainCameraEntity(ECS::Registry& registry);
		static CameraComponent* GetMainCamera(ECS::Registry& registry);
		static TransformComponent* GetMainCameraTransform(ECS::Registry& registry);
};
}

