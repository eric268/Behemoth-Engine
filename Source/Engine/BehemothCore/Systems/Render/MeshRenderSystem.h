#pragma once

#include "RenderSystem.h"
#include <tuple>
#include <vector>
#include <mutex>

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
		void ProcessMesh(Mesh& mesh, TransformComponent* cameraTransform, const BMath::Matrix4x4f& transform, const BMath::Matrix4x4f& viewProjMatrix, bool dirty, int renderSlotIndex);
		bool CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector3 forwardVec,  const BMath::Vector4 primitiveVerts[]);
		void AddPrimitiveToRenderer(Primitive& primitive, const int numVerticies, const BMath::Vector4 verticies[], int index);
		float GetPrimitiveDepth(const int numVerticies, const BMath::Vector4 verticies[]);

		std::mutex counterMux;
	};
}