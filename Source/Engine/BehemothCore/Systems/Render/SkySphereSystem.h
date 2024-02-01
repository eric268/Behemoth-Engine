#pragma once

#include "Systems/Render/MeshRenderSystem.h"


namespace ECS
{
	class Registry;
}

// Class attempts to mimic a cube-map by using a sphere
// There were issues with artifacts and primitive distortion when a cube was used
namespace Behemoth
{
	struct SkySphereComponent;
	struct TransformComponent;

	class SkySphereSystem : public MeshRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void FollowCamera(TransformComponent* transformComp, const BMath::Vector3& cameraPosition);
		void InitializeSphere(SkySphereComponent* skySphereComp);
		void ProcessSphere(
			TransformComponent* transformComp,
			SkySphereComponent* skySphereComp,
			TransformComponent* cameraTransform,
			const BMath::Matrix4x4& viewProjMatrix,
			int renderSlotIndex);
	};

}

