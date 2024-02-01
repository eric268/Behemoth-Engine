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
		void InitializeProjMatrix(CameraComponent* cameraComp, const TransformComponent* transformComp);
		void UpdateFrustrum(CameraComponent* cameraComp, TransformComponent* transformComp);
		void SetLook(ECS::Registry& registry, CameraComponent* cameraComp, TransformComponent* cameraTransformComp);
	};

}