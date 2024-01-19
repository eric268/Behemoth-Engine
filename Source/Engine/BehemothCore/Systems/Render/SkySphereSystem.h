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
		void FollowCamera(TransformComponent* transform, const BMath::Vector3& cameraPosition);
		void InitalizeSphere(SkySphereComponent* skySphereComponent);
		void ProcessSphere(
			TransformComponent* transformComp,
			SkySphereComponent* skySphereComponent,
			TransformComponent* cameraTransform,
			const BMath::BMatrix4x4& viewProjMatrix,
			int renderSlotIndex);

		void ProcessPrimitive(
			TransformComponent* transform,
			SkySphereComponent* skySphere,
			TransformComponent* cameraTransform,
			const BMath::BMatrix4x4& viewProjMatrix,
			int primitiveIndex,
			int vertexIndex,
			int numVerticies);
	};

}

