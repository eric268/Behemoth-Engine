#pragma once

#include "Math/MathCore.h"

class CameraHelper
{
public:

	static Math::Matrix4x4 LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up);
};

