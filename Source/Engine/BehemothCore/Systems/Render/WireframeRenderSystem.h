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
		void ProcessWireframe(Mesh& mesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transform, const Math::Matrix4x4& viewProjMatrix, bool dirty);
		void AddWireMeshToRenderer(const int numVerticies, const Math::Vector4 verticies[]);
		void ReserveResources(int numPrimitives) override;
	};
}
