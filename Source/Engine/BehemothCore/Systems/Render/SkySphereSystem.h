#pragma once

#include "Systems/Render/MeshRenderSystem.h"


namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct SkySphereComponent;
	struct CameraComponent;
	struct TransformComponent;

	class SkySphereSystem : public MeshRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void FollowCamera(TransformComponent* transform, const BMath::Vector3& cameraPosition);
		void InitalizeSphere(SkySphereComponent* skySphereComponent);
		void ProcessSphere(TransformComponent* transformComp, SkySphereComponent* skySphereComponent, const BMath::Matrix4x4f& viewProjMatrix, int renderSlotIndex);
	};

}

