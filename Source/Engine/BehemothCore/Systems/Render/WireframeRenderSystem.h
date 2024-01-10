#pragma once

#include "RenderSystem.h"

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;

	class WireframeRenderSystem : public RenderSystem
	{
	public:
		WireframeRenderSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry) override;
	
	protected:
		void ProcessWireframe(Mesh& mesh, const BMath::Matrix4x4& transform, const BMath::Matrix4x4& viewProjMatrix, bool dirty = true, BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));
		void AddWireMeshToRenderer(const int numVerticies, const BMath::Vector4 verticies[], BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));
		void ReserveResources(int numPrimitives) override;
		BMath::Matrix4x4 GetWireframeTransform(const BMath::Matrix4x4& ownerTransform, const BMath::Vector3& ownerWorldScale, const BMath::Vector3& wireframeScale, const bool allowRotation);
	};
}
