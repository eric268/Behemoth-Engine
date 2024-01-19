#pragma once

#include "RenderSystem.h"
#include <tuple>
#include <vector>

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;
	class Entity;

	class MeshRenderSystem : public RenderSystem
	{
	public:
		MeshRenderSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry) override;

	protected:
		void ReserveResources(int numPrimitives) override;
		bool CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector3 forwardVec,  const BMath::Vector4 primitiveVerts[]);
		void AddPrimitiveToRenderer(Primitive& primitive, const int numVerticies, const BMath::Vector4 verticies[], int index);
		float GetPrimitiveDepth(const int numVerticies, const BMath::Vector4 verticies[]);

		void ProcessMesh(
			Mesh& mesh,
			TransformComponent* cameraTransform,
			const BMath::BMatrix4x4& transform, 
			const BMath::BMatrix4x4& viewProjMatrix,
			bool dirty,
			int renderSlotIndex);
	};
}