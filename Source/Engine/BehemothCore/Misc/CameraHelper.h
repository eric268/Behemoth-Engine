#pragma once

#include "Math/MathCore.h"
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

	class CameraHelper
	{
	public:
		static Math::Matrix4x4 LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up);
		static CameraComponent* GetMainCamera(ECS::Registry& registry);
		static Math::Vector3 GetMainCameraPostition(ECS::Registry& registry);
};
}

