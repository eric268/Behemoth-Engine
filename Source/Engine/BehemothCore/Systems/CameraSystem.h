#pragma once

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;

	class CameraSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitializeProjMatrix(CameraComponent* component, const TransformComponent* transformComponent);
		void UpdateFrustrum(CameraComponent* cameraComponent, TransformComponent* transform);
		void SetLook(ECS::Registry& registry, TransformComponent* cameraTransform, CameraComponent* cameraComponent);
	};

}