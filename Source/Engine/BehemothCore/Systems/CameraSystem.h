#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;
	class Matrix4x4;

	class CameraSystem
	{
	public:
		CameraSystem() {};
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void UpdatePerspectiveMatrix(CameraComponent* component, const TransformComponent* transformComponent);
		void UpdateFrustrum(CameraComponent* cameraComponent);

		float GetDistance(const BMath::Matrix4x4& m, int index);
	};

}