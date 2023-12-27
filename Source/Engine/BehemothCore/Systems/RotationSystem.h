#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		Math::Vector3 GetForwardVector(const Math::Matrix4x4& transformMatrix);
		Math::Vector3 GetRightVector(const Math::Matrix4x4& transformMatrix);
	};
}
