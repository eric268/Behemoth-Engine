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
		void ProcessBoundingVolume(BoundingVolumeComponent* boundingVolumeComponent, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix);
	};
}

