#pragma once

#include "RenderSystem.h"

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;
	class Mesh;

	class MeshRenderSystem : public RenderSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry) override;

	protected:
		void ReserveResources(int numPrimitives) override;
		bool CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector3 forwardVec,  const BMath::Vector4 primitiveVerts[]);
		void AddPrimitiveToRenderer(Primitive& primitive, const int numVertices, const BMath::Vector4 vertices[], int index);
		float GetPrimitiveDepth(const int numVertices, const BMath::Vector4 vertices[]);

		void ProcessMesh(
			Mesh& mesh,
			TransformComponent* cameraTransform,
			const BMath::Matrix4x4& transform, 
			const BMath::Matrix4x4& viewProjMatrix,
			const BMath::Vector3& worldForwardVector,
			bool dirty,
			int renderSlotIndex);
	};
}