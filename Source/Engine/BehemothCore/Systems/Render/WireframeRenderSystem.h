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
		void ProcessWireframe(Mesh& mesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transform, const Math::Matrix4x4& viewProjMatrix, const Math::Vector3& scale, bool dirty = true, Math::Vector3 color = Math::Vector3(1.0f, 1.0f, 1.0f));
		void AddWireMeshToRenderer(const int numVerticies, const Math::Vector4 verticies[], Math::Vector3 color = Math::Vector3(1.0f, 1.0f, 1.0f));
		void ReserveResources(int numPrimitives) override;
		Math::Matrix4x4 GetScaledTransformMatrix(const Math::Matrix4x4& transformMatrix, const Math::Vector3& scale);
	};
}
