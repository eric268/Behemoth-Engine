#pragma once
#include "WireframeRenderSystem.h"

namespace Behemoth
{
	class BoundingVolumeComponent;

	class BoundingVolumeRenderSystem : public WireframeRenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;

	private:
		void ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, const BMath::Vector3 cameraPosition, const BMath::Matrix4x4& transformMatrix, const BMath::Matrix4x4& viewProjMatrix);
	};
}

