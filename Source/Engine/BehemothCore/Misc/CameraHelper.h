#pragma once

#include "Math/MathCore.h"

namespace ECS 
{
	class CameraComponent;
}


class CameraHelper
{
public:
	static Math::Matrix4x4 LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up);
	static Math::Vector3 GetPosition(const ECS::CameraComponent* component);
};

