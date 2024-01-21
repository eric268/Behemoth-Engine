#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;
	class Matrix4x4;

	class CameraSystem
	{
	public:
		CameraSystem() {};
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitalizeProjMatrix(CameraComponent* component, const TransformComponent* transformComponent);
		void UpdateFrustrum(CameraComponent* cameraComponent, TransformComponent* transform);
		void SetLook(ECS::Registry& registry, TransformComponent* cameraTransform, CameraComponent* cameraComponent);
	};

}