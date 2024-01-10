#pragma once
#include "WireframeRenderSystem.h"

namespace Behemoth
{
	class BoundingVolumeComponent;
	class TransformComponent;

	class BoundingVolumeRenderSystem : public WireframeRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;

	private:
		void ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, TransformComponent* transform, const BMath::Vector3 cameraPosition, const BMath::Matrix4x4& viewProjMatrix);
	};
}

