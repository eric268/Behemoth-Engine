#pragma once

#include "Math/MathCore.h"
#include "ECS/Components.h"

class CameraHelper
{
public:

	static Math::Matrix4x4 LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up);
	static Math::Vector3 GetCameraLocation(const ECS::CameraComponent& component);
};

