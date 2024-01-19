#pragma once
#include "WireframeRenderSystem.h"

namespace Behemoth
{
	struct BoundingVolumeComponent;
	struct TransformComponent;

	class BoundingVolumeRenderSystem : public WireframeRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;

	private:
		void ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, TransformComponent* transform, const BMath::Vector3 cameraPosition, const BMath::BMatrix4x4& viewProjMatrix);
	};
}

