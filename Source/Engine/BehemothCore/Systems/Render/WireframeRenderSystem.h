#pragma once

#include "RenderSystem.h"

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;
	class Mesh;

	// This is just a system used for debugging primarily colliders, but anything mesh related
	// It will not run unless in DEBUG 
	class WireframeRenderSystem : public RenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;
	
	protected:
		void ProcessWireframe(
			Mesh& mesh,
			const BMath::Matrix4x4& transform,
			const BMath::Matrix4x4& viewProjMatrix,
			bool dirty = true,
			BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));

		void AddWireMeshToRenderer(
			const int numVertices,
			const BMath::Vector4 vertices[],
			BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f));

		BMath::Matrix4x4 GetWireframeTransform(
			const BMath::Matrix4x4& ownerTransform,
			const BMath::Vector3& ownerWorldScale,
			const BMath::Vector3& wireframeScale,
			const bool allowRotation);

		void ReserveResources(int numPrimitives) override;
	};
}
