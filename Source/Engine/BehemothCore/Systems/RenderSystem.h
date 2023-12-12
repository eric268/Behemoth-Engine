#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	class CameraComponent;
	class FrustrumComponent;
	class TransformComponent;
	class Mesh;

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	private:
		void DrawMesh(Mesh& mesh, bool isVisible, bool drawWireFrame, const Math::Vector3 cameraPosition, const Math::Matrix4x4 & meshTransform, const Math::Matrix4x4& viewProjMatrix);
		void DrawBoundingVolume(Mesh& mesh, const float radius, const Math::Vector3& cameraPosition, const Math::Matrix4x4& meshTransform, const Math::Matrix4x4& viewProjMatrix);

		bool CullQuadBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
		bool CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
		bool IsPrimitiveWithinFrustrum(const int numVerticies, Math::Vector4 primitiveVerts[]);
		bool IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const FrustrumComponent* frustrumComp, const TransformComponent* transformComp, const float boundingRadius);
	};
}